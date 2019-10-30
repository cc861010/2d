#ifndef __TILESET_H__
#define __TILESET_H__

#include "json/json.h"
#include <iostream>
#include <vector>

#include <fstream>
#include <streambuf>
#include <string>
#include <cerrno>
namespace tile {


std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    return(contents.str());
  }
  throw(errno);
}

int string_to_json(const std::string& rawJson,Json::Value& root){
    const int rawJsonLength = static_cast<int>(rawJson.length());
    // constexpr bool shouldUseOldWay = false;
    JSONCPP_STRING err;
    // if (shouldUseOldWay) {
    //     Json::Reader reader;
    //     reader.parse(rawJson, root);
    // } else {
        Json::CharReaderBuilder builder;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength,&root,&err)) {
            std::cout << "error" << std::endl;
            return EXIT_FAILURE;
        }
    // }
    return EXIT_SUCCESS;
}

std::string json_to_string(const Json::Value& root) {
//   constexpr bool shouldUseOldWay = false;
//   if (shouldUseOldWay) {
//     Json::FastWriter writer;
//     const std::string rawJson = writer.write(root);
//     //std::cout << rawJson << std::endl;
//     return rawJson;
//   } else {
    Json::StreamWriterBuilder builder;
    const std::string rawJson = Json::writeString(builder, root);
    //std::cout << rawJson << std::endl;
    return rawJson;
//   }
}

    struct property {
        std::string name;
        std::string type;
        std::string value;

        static Json::Value object_to_json(const property& p){
            Json::Value root;
            root["name"] = Json::Value(p.name);
            root["type"] = Json::Value(p.type);
            root["value"] = Json::Value(p.value);
            return root;
        }
        static property json_to_object(const Json::Value& root){
            property p;
            p.name =  root["name"].asString();
            p.type =  root["type"].asString();
            p.value = root["value"].asString();
            return p;
        }
        std::string to_string(){
            return json_to_string(object_to_json(*this));
        }
    };



    struct frame {
        //Frame duration in milliseconds
        int duration;		
        //Local tile ID representing this frame
        int tileid;		

        static Json::Value object_to_json(const frame& f){
            Json::Value root;
            root["duration"] = f.duration;
            root["tileid"] = f.tileid;
            return root;
        }
        static frame json_to_object(const Json::Value& root){
            frame f;
            f.duration = root["duration"].asInt();
            f.tileid = root["tileid"].asInt();
            return f;
        }
        std::string to_string(){
            return json_to_string(object_to_json(*this));
        }
    };


    struct tile {
        //Array of Frames
        std::vector<frame> animation;
        //Local ID of the tile
        int id;	
        //Image representing this tile (optional)
        std::string image;		
        //Height of the tile image in pixels
        int imageheight;		
        //Width of the tile image in pixels
        int imagewidth;	
        //Layer with type objectgroup (optional)
        //Layer objectgroup;		
        //A list of properties (name, value, type)
        std::vector<property> properties;
        //Index of terrain for each corner of tile
        //tile terrain[]	
        //The type of the tile (optional)
        std::string type;

        static Json::Value object_to_json(const tile& t){
            Json::Value root;
            Json::Value animation;
            std::vector<frame> animation_object = (std::vector<frame>)(t.animation);
            if(animation_object.size() != 0){
                for (int i = 0; i < animation_object.size(); i++){
                    animation.append(frame::object_to_json(animation_object[i]));
                }
                root["animation"] = animation;
            }
            root["id"] = t.id;
            root["image"] = t.image;
            root["imageheight"] = t.imageheight;
            root["imagewidth"] = t.imagewidth;
            //todo 
            Json::Value properties(Json::arrayValue);
            std::vector<property> v = (std::vector<property>)(t.properties);
            for (int i = 0; i < v.size(); i++){
                properties.append(property::object_to_json((property)(v[i])));
            }
            //for (std::vector<property>::iterator it = ((std::vector<property>)(t.properties)).begin(); it != ((std::vector<property>)(t.properties)).end(); it++){
                //properties.append(property::object_to_json(it));
                // Json::Value v(Json::objectValue);
                // v["name"] = (property)(*it)).name;
                // v["type"] = (property)(*it)).type;
                // v["value"] = (property)(*it)).value;
                //properties.append(v);
            //}
            root["properties"] = properties;
            root["type"] = t.type;
            return root;
        }
        static tile json_to_object(const Json::Value& root){
            tile t;
            Json::Value arrayValue = root["animation"];
            if(arrayValue.size()!=0){
                t.animation = std::vector<frame>();
                for (Json::Value::const_iterator it = arrayValue.begin(); it != arrayValue.end(); ++it) {
                    t.animation.push_back(frame::json_to_object(*it));
                }
            }
            t.id = root["id"].asInt();
            t.image = root["image"].asString();
            t.imageheight = root["imageheight"].asInt();
            t.imagewidth = root["imagewidth"].asInt();
            t.properties = std::vector<property>();
            Json::Value arrayValue1 = root["properties"];
            for (Json::Value::const_iterator it = arrayValue1.begin(); it != arrayValue1.end(); ++it) {
                t.properties.push_back(property::json_to_object(*it));
            }
            t.type = root["type"].asString();
            return t;
        }
        std::string to_string(){
            return json_to_string(object_to_json(*this));
        }
    };


    struct tileset {
        //The number of tile columns in the tileset
        int columns;		
        //GID corresponding to the first tile in the set
        int firstgid;		
        //See <grid> (optional)
        //grid grid_object;		
        //Image used for tiles in this set
        std::string image;		
        //Width of source image in pixels
        int imagewidth;		
        //Height of source image in pixels
        int imageheight;		
        //Buffer between image edge and first tile (pixels)
        int margin;		
        //Name given to this tileset
        std::string name;		
        //A list of properties (name, value, type).
        std::vector<property> properties;		
        //Spacing between adjacent tiles in image (pixels)
        int spacing;		
        //Array of Terrains (optional)
        //terrain terrains[];		
        //The number of tiles in this tileset
        int tilecount;	
        //Maximum width of tiles in this set
        int tilewidth;	
        //Maximum height of tiles in this set
        int tileheight;		
        //See <tileoffset> (optional)
        int offsetx;	
        int offsety;		
        //Array of Tiles (optional)
        std::vector<tile> tiles;		
        //Hex-formatted color (#RRGGBB) (optional)
        std::string transparentcolor;		
        //tileset (for tileset files, since 1.0)
        std::string type;	
        //Array of Wang sets (since 1.1.5)
        //wangset wsets;	

        static Json::Value object_to_json(const tileset& t){
            Json::Value root;
            root["columns"] = t.columns;
            root["firstgid"] = t.firstgid; 
            root["image"] = t.image; 
            root["imagewidth"] = t.imagewidth; 
            root["imageheight"] = t.imageheight; 
            root["margin"] = t.margin; 
            root["spacing"] = t.spacing; 
            root["tilecount"] = t.tilecount; 
            root["tileheight"] = t.tileheight; 
            root["tilewidth"] = t.tilewidth; 
            root["offsetx"] = t.offsetx; 
            root["offsety"] = t.offsety; 
            root["transparentcolor"] = t.transparentcolor; 
            root["type"] = t.type; 
            root["name"] = t.name; 
            Json::Value properties(Json::arrayValue);
            std::vector<property> ps = t.properties;
            for(int i=0;i<ps.size();i++){
                properties.append(property::object_to_json(ps[i]));
            }
            root["properties"] = properties;

            Json::Value tiles_v;
            std::vector<tile> tilesArray = t.tiles;
            //std::cout << "||" << t.tiles.size() << "||";
            for(int i=0;i<t.tiles.size();i++){
                Json::Value v11 = tile::object_to_json(t.tiles[i]);
                tiles_v.append(v11);
                //std::cout << "\n||" << v11 << "||\n" ;
                //Json::Value t_v ;//= tile::object_to_json(t.tiles[i]);
                //tiles_v.append(t_v);
                //tiles_v.append(Json::Value(1));
            }
            root["tiles"] = tiles_v;
            // for(int i=0;i<tiles_v.size();i++){
            //     std::cout << "||" << tiles_v[i] << "||";
            // }
            return root;
        }
        static tileset json_to_object(const Json::Value& root){
            tileset t;
            t.columns = root["columns"].asInt(); 
            t.firstgid = root["firstgid"].asInt(); 
            t.imagewidth = root["imagewidth"].asInt(); 
            t.imageheight = root["imageheight"].asInt(); 
            t.margin = root["margin"].asInt(); 
            t.spacing = root["spacing"].asInt(); 
            t.tilecount = root["tilecount"].asInt(); 
            t.tileheight = root["tileheight"].asInt(); 
            t.tilewidth = root["tilewidth"].asInt(); 
            t.offsetx = root["offsetx"].asInt(); 
            t.offsety = root["offsety"].asInt(); 
            t.name = root["name"].asString(); 
            t.type = root["type"].asString(); 
            t.image = root["image"].asString();
            t.transparentcolor = root["transparentcolor"].asString();
            Json::Value properties = root["properties"];
            t.properties = std::vector<property>();
            for(int i=0;i<properties.size();i++){
                t.properties.push_back(property::json_to_object(properties[i]));
            }
            Json::Value tiles = root["tiles"];
            for(int i=0;i<tiles.size();i++){
                t.tiles.push_back(tile::json_to_object(tiles[i]));
                //std::cout << "||" << tiles[i] << "||";
                //std::cout << "||" << tile::json_to_object(tiles[i]).to_string() << "||";
            }
            return t;
        }
        std::string to_string(){
            return json_to_string(object_to_json(*this));
        }
    };


}


#endif