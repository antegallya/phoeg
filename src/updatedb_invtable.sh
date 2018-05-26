#!/bin/bash
set -e # Exit on error.
set -u # Error on unset variable use.

function usage {
  echo "Usage: $0 [graphs] [inv_table] [inv_comp_prog]"
  exit 1
}

[ $# -eq 3 ] || usage

graphs=$1
inv_table=$2
inv_comp_prog=$3

function comp_inv {
  in=$1
  out=$2
  parallel --pipe-part --block -100 -a "$in" --eta -j 12 --workdir . "$inv_comp_prog" > $out
}

# Set these environmental variables to override them,
# but they have safe defaults.
export PGHOST=${PGHOST-localhost}
export PGPORT=${PGPORT-5432}
export PGDATABASE=${PGDATABASE-my_database}
export PGUSER=${PGUSER-my_user}
export PGPASSWORD=${PGPASSWORD-my_password}

RUN_PSQL="psql -X --set AUTOCOMMIT=off --set ON_ERROR_STOP=on --set ECHO=all"

GET_PSQL="psql -X --set AUTOCOMMIT=off --set ON_ERROR_STOP=on \
  -P t -P format=unaligned"

unset_sigs_file=$(tempfile)
inv_values_file=$(tempfile)
clean=true
function cleanup {
  if [ "$clean" = true ]
  then
    rm -f $unset_sigs_file
    rm -f $inv_values_file
  fi
}
function die {
  echo "> ERROR: $@"
  exit 1
}
function onerror {
  echo "> There was en error. There are two temporary files :"
  echo "> unset signatures: $unset_sigs_file"
  echo "> invariant values: $inv_values_file"
  read -p "> Do you want me to cleanup the temporary files (Y/n) ?:" ans
  case $ans in
    "n"|"N"|"n*"|"N*")
      clean=false
      die "Exit keeping the temporary files";;
  esac
}
trap cleanup EXIT
trap onerror ERR

echo "> Update values of $inv_table not in $graphs using $inv_comp_prog."
table_exists=$(
${GET_PSQL} <<SQL
  SELECT EXISTS (
     SELECT 1
     FROM   information_schema.tables 
     WHERE  table_schema = 'public'
     AND    table_name = '$inv_table'
     )
SQL
)
if [ "$table_exists" != "t" ]
then
  read -p "> Table $inv_table doesn't exist create it (y/N) ?: " ans
  case $ans in
    "y"|"Y"|"y*"|"Y*")
      read -p "> What is the invariant type ?: " itype
      [ $itype == "integer" ] || [ $itype == "boolean" ] || [ $itype == "numeric" ] || die "Invalid type"
  ${RUN_PSQL} <<SQL
  CREATE TABLE public.$inv_table
  (
    signature text NOT NULL,
    val integer,
    CONSTRAINT pk_$inv_table PRIMARY KEY (signature)
  )
  WITH (
    OIDS=FALSE
    );
  COMMIT;
SQL
    ;;
    *) die "Table does not exist."
  esac
fi

echo "> Get unset invariant values."
${RUN_PSQL} <<SQL
\timing on
  CREATE TEMPORARY TABLE unset_signatures
  AS
  SELECT signature FROM $graphs g
  WHERE NOT EXISTS (SELECT 1 FROM $inv_table i WHERE i.signature = g.signature);
\COPY unset_signatures TO '$unset_sigs_file' WITH CSV DELIMITER E'\t';
SQL

echo "> Compute invariants values."
time comp_inv "$unset_sigs_file" "$inv_values_file"

echo "> Import invariants values in DB."
${RUN_PSQL} <<SQL
\timing on
  CREATE TEMPORARY TABLE tmp_table
  AS
  SELECT * 
  FROM $inv_table
  WITH NO DATA;
\copy tmp_table FROM '$inv_values_file' WITH CSV DELIMITER E'\t';
  INSERT INTO $inv_table
  SELECT *
  FROM tmp_table
  ON CONFLICT DO NOTHING;
  COMMIT;
SQL
