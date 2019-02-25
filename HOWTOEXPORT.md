In case you forget (that's, everytime you try to do it). Here is an exemple :

cat all10 | src/output_inv > my_invariant_all10

Then start psql
===============
psql -U graphedron -d graphedrondb -h localhost

Create the table if needed :
============================
CREATE TABLE min_deg
(
signature text NOT NULL,
val integer,
CONSTRAINT pk_min_deg PRIMARY KEY (signature),
CONSTRAINT fk_min_deg FOREIGN KEY (signature)
REFERENCES graphs (signature) MATCH SIMPLE
ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
OIDS=FALSE
);
ALTER TABLE min_deg
OWNER TO graphedron;

and copy the data in the table :
================================
\copy my_invariant_table FROM 'my_invariant_all10' WITH CSV DELIMITER E'\t';

ANALYZE my_invariant_table;
===========================


If you want to add a new class of graphs :
==========================================
We need to add them, if needed, to the global graphs table. For that, we import
the data in the table, without the FK constraint. Insert it in the global table
afterwards. And finally add the FK.

\set classtable conn_4regular
\set file out

CREATE TABLE :classtable
AS
SELECT * 
FROM graphs
WITH NO DATA;

\copy conn_4regular FROM out WITH CSV DELIMITER E'\t';

ALTER TABLE :classtable
ADD CONSTRAINT pk_:classtable PRIMARY KEY (signature);

INSERT INTO graphs
SELECT *
FROM :classtable
ON CONFLICT DO NOTHING;

ALTER TABLE :classtable
ADD CONSTRAINT fk_:classtable FOREIGN KEY (signature)
REFERENCES graphs (signature) MATCH SIMPLE;

If you need to add invariants values of new graphs :
====================================================
**Automagically use dbexport/updatedb_invtable.sh**

bash updatedb_invtable.sh graphs min_deg ../phoeg/src/min_deg


Since some values might already be in the table, create a temporary table, import the data into it and merge it in the invariant table.

\set invtable num_vertices

CREATE TEMPORARY TABLE tmp_table
AS
SELECT * 
FROM :invtable
WITH NO DATA;

\copy tmp_table FROM 'out_inv' WITH CSV DELIMITER E'\t';

INSERT INTO :invtable
SELECT *
FROM tmp_table
ON CONFLICT DO NOTHING; DROP TABLE tmp_table;

Need to seek what wasn't computed yet ?
=======================================
\set bigtable graphs
\set smalltable semi_total_dom_num

\copy (SELECT signature FROM graphs g WHERE NOT EXISTS (SELECT 1 FROM min_deg n WHERE n.signature = g.signature )) TO 'out' WITH CSV DELIMITER E'\t';
