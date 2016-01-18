#include "Heuristic.hh"

bool                    Heuristic::operator<=(Heuristic::stateNode const& l, Heuristic::stateNode const& r) {

  return l.score <= r.score; 
}
