#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}

//=======================//
//          TODO         //
//=======================//
void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{
    int ctrl = 0;
    const std::vector<GraphVertex>& vertexList = navigationMap.GetVertexList();
    for (int i = 0; i < vertexList.size(); ++i){
        const GraphVertex& vertex = vertexList[i];

        for (int j = 0; j < vertex.edges.size(); ++j){
            const GraphEdge& edge = vertex.edges[j];

            if (edge.name == airlineName &&
                vertex.name == airportFrom &&
                vertexList[edge.endVertexIndex].name == airportTo){
                //store it to the vector
                HaltedFlight haltedFlight;
                haltedFlight.airportFrom = airportFrom;
                haltedFlight.airportTo = airportTo;
                haltedFlight.airline = airlineName;
                haltedFlight.w0 = edge.weight[0];
                haltedFlight.w1 = edge.weight[1];
                haltedFlights.push_back(haltedFlight);
                //remove form graph (remove yanlış tekrar bak)
                navigationMap.RemoveEdge(airlineName, airportFrom, airportTo);
                ctrl = 1;
                break;
            }
        }

        if (ctrl == 1) break;
    }

    if (ctrl == 0) PrintCanNotHalt(airportFrom, airportTo, airlineName);
    
    /* TODO */
}

    // (Direct Function call)
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    int ctrl = 0;
    for (int i = 0; i<haltedFlights.size();i++){
        if(haltedFlights[i].airportFrom == airportFrom &&
           haltedFlights[i].airportTo == airportTo &&
           haltedFlights[i].airline == airlineName){
               navigationMap.AddEdge(haltedFlights[i].airline,haltedFlights[i].airportFrom,haltedFlights[i].airportTo,haltedFlights[i].w0,haltedFlights[i].w1);
               haltedFlights.erase(haltedFlights.begin()+i);
               ctrl = 1;
         }
         if (ctrl == 1){
             break;
         }
    }
    if(ctrl == 0){
        PrintCanNotResumeFlight(airportFrom, airportTo, airlineName);
    }
    /* TODO */
}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    
    bool isCostWeighted = (alpha == 0.0f || alpha == 1.0f);

    // cache
    std::vector<int> orderedVertexEdgeIndexList;
    if (lruTable.Find(orderedVertexEdgeIndexList, navigationMap.GetVertexIndex(startAirportName),
                      navigationMap.GetVertexIndex(endAirportName), isCostWeighted, true))
    {
        
        PrintFlightFoundInCache(startAirportName, endAirportName, isCostWeighted);
        navigationMap.PrintPath(orderedVertexEdgeIndexList, alpha, true);
    }
    else
    {
        
        if (navigationMap.HeuristicShortestPath(orderedVertexEdgeIndexList, startAirportName, endAirportName, alpha))
        {
            
            if (isCostWeighted == 1){
            PrintFlightCalculated(startAirportName, endAirportName, isCostWeighted);
            }
            navigationMap.PrintPath(orderedVertexEdgeIndexList, alpha, true);

            
            if (isCostWeighted)
            {
                lruTable.Insert(orderedVertexEdgeIndexList, isCostWeighted);
            }
        }
        else
        {
            
            PrintPathDontExist(startAirportName, endAirportName);
        }
    }
    /* TODO */
}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{

    bool isCostWeighted = (alpha == 0.0f || alpha == 1.0f);

    
    std::vector<int> orderedVertexEdgeIndexList;
    
    
        
        if (navigationMap.FilteredShortestPath(orderedVertexEdgeIndexList, startAirportName, endAirportName, alpha, unwantedAirlineNames))
        {
            
            if (isCostWeighted == 1){
            PrintFlightCalculated(startAirportName, endAirportName, isCostWeighted);
            }
            navigationMap.PrintPath(orderedVertexEdgeIndexList, alpha, true);

            
        }
        else
        {
            
            PrintPathDontExist(startAirportName, endAirportName);
        }
    
    /* TODO */
    /* TODO */
}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    /* TODO */
}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    
    int ctrl = 0;
    for (int i = 0; i < navigationMap.GetVertexList2().size(); ++i){
        if(navigationMap.GetVertexList2()[i].name == airportName){
            ctrl = 1;
        }
    }
    if(ctrl == 0) return -1;
    else{
        return navigationMap.MaxDepthViaEdgeName(airportName , airlineName);
    }
}