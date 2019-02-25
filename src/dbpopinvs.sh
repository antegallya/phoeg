#!/bin/bash
comp_path="$HOME/phoeg/src/invariants_mono"
while read invd
do
  read -ra INVD <<< "$invd"
  inv_comp_prog="$comp_path/${INVD[0]}"
  inv_table="${INVD[1]}"
  inv_type="${INVD[2]}"
  echo "INV comp:$inv_comp_prog table: $inv_table type: $inv_type"
  updatedb_invtable graphs $inv_table $inv_comp_prog $inv_type
done << INVS
totalEccentricity total_eccentricity integer
order num_vertices integer
numEdges num_edges integer
wienerIndex wiener_index integer
averageDistance average_distance numeric
diameter diam integer
radius radius integer
maxDegree max_deg integer
minDegree min_deg integer
isConnected is_connected boolean
isTree is_tree boolean
eccentricConnectivity eccentric_connectivity_index integer
semiTotalDominationNumber semi_total_dom_num integer
dominationNumber dom_num integer
totalDominationNumber total_dom_num integer
clawFree clawfree boolean
minVertexCover min_vertex_cover integer
chromaticNumber chromatic_number integer
maxIndependentSet max_indep_set integer
isPlanar is_planar boolean
numCol num_col integer
INVS
