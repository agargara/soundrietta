#pragma once

#include <string>
#include <vector>

using namespace std;



class NLTmxMapTileset {
public:
    int firstGid;
    string name;
    int tileWidth;
    int tileHeight;
    string filename;
};


class NLTmxMapLayer {
public:
    string name;
    int width;
    int height;
    int* data;
    
    ~NLTmxMapLayer() {
        delete [] data;
    }
};


struct NLTmxMapObjectProperty {
    string name;
    string value;
};


class NLTmxMapObject {
public:
    string name;
    int gid;
    int x;
    int y;
    int width;
    int height;
    vector<NLTmxMapObjectProperty*> properties;
    
public:
    
    ~NLTmxMapObject() {
        for ( auto property : properties ) {
            delete property;
        }
    }
};


class NLTmxMapObjectGroup {
public:
    string name;
    int width;
    int height;
    bool visible;
    
    vector<NLTmxMapObject*> objects;
    
    ~NLTmxMapObjectGroup() {
        for ( auto o : objects ) {
            delete o;
        }
    }
};


class NLTmxMap {
public:
    
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    
    vector<NLTmxMapTileset*> tilesets;
    vector<NLTmxMapLayer*> layers;
    vector<NLTmxMapObjectGroup*> groups;
    
    ~NLTmxMap() {
        for ( auto g : groups ) {
            delete g;
        }
        
        for ( auto l : layers ) {
            delete l;
        }
        
        for ( auto ts : tilesets ) {
            delete ts;
        }
    }
    
    
};


NLTmxMap* NLLoadTmxMap( char *xml );

