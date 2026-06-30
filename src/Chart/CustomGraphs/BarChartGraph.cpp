#pragma once
#include <src/Chart/GraphFactoryRegistry.h>
#include <src/Chart/CustomGraphs/BarChartGraph.h>

REGISTER_GRAPH_FACTORY("Bar", BarChartGraph)
