//
// Created by rolf on 05-08-20.
//

#ifndef SOCCER_OBJECTFILTER_H
#define SOCCER_OBJECTFILTER_H

//TODO: fix this giving actually unique ID's..
class ObjectFilter {
public:
    ObjectFilter();
    [[nodiscard]] int getObjectID() const;
    void setObjectID(int id);

private:
    static int nextObjectID;
    int objectID;
};


#endif //SOCCER_OBJECTFILTER_H
