import osmnx as ox
import matplotlib
import geopandas as gpd



# define a bounding box in San Francisco
north, south, east, west = 47.4859, 47.4672, 19.0807, 19.0340

# create network from that bounding box
G = ox.graph_from_bbox(north, south, east, west, network_type="drive_service")

hwy_speeds = {"residential": 30, "secondary": 50, "tertiary": 60}
G = ox.add_edge_speeds(G, hwy_speeds)
G = ox.add_edge_travel_times(G)

G_proj = ox.project_graph(G)
ints = ox.consolidate_intersections(G_proj, rebuild_graph=True, tolerance=1, dead_ends=True, reconnect_edges=True)

fig, ax = ox.plot_graph(
    ints, node_color="r", node_size=5, edge_color="w", edge_linewidth=1
)
intsP = ox.project_graph(ints, to_latlong=True)
b = ox.stats.intersection_count(ints, min_streets=0)
M = ox.utils_graph.get_undirected(intsP)
print(b)
att_list = ["highway","service","ref","osmid","geometry","from","to", "tunnel", "bridge","width", "access", "v_original", "u_original", "speed_kph", "reversed", "maxspeed", "lanes", "oneway", "name"]
for n1, n2, d in M.edges(data=True):
    for att in att_list:
        d.pop(att, None)
ox.save_graphml(M)
