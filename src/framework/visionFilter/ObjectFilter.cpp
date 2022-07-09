//
// Created by rolf on 05-08-20.
//

#include "ObjectFilter.h"
int ObjectFilter::nextObjectID = 0;
int ObjectFilter::getObjectID() const {
    return objectID;
}

void ObjectFilter::setObjectID(int id) {
    objectID = id;
}
ObjectFilter::ObjectFilter() {
  {
    objectID = nextObjectID;
    nextObjectID++;
  }
}
