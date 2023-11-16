import osmnx as ox
import matplotlib
import geopandas as gpd



# define a bounding box in San Francisco
north, south, east, west = 47.4852, 47.4689, 19.0651, 19.0503

# create network from that bounding box
G = ox.graph_from_place("Lágymányos", network_type="drive_service")

hwy_speeds = {"residential": 30, "secondary": 50, "tertiary": 60}
G = ox.add_edge_speeds(G, hwy_speeds)
G = ox.add_edge_travel_times(G)

ints = ox.consolidate_intersections(G, tolerance=15, rebuild_graph=True, dead_ends=True, reconnect_edges=True) 
fig, ax = ox.plot_graph(
    ints, node_color="r", node_size=5, edge_color="w", edge_linewidth=1
)


b = ox.stats.intersection_count(ints, min_streets=0)
print(b)
ox.save_graphml(ints)
