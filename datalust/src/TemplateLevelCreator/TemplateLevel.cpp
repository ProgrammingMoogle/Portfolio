/*************************************************************************/
/*!
\file   TemplateLevel.h
\author Studio200
\par
\par    DigiPen login:
\par    Course: GAME 200
\par   Copyright � 2017 DigiPen (USA) Corp. and its owners. All Rights Reserved.
\date   9/28/2017
\brief
Functions include:
*/
/*************************************************************************/
#include "TemplateLevel.h"
#include <fstream>
#include <ios>
#include "Engine/Engine.h"
#include "Transform/Transform.hpp"
#include "Graphics/Camera.hpp"
#include <cctype>
#include <ratio>

/**
 * \brief 
 * \param filename 
 * \param obj 
 */
void Read_Buffer(const char * filename, Json::Value * obj)
{
  std::ifstream inFile(filename);
  if (!inFile.is_open())
  {
    std::string err = filename;
    err + " not found";
    throw std::exception(err.c_str());
  }

  // create a Json Char Reader to read the filebuffer into a json value
  Json::Reader Parser;
  // create a string to store errors from parse
  // read the file into the jason value
  Parser.parse(inFile, *obj);
}

/**
 * \brief 
 * \param file 
 */
TemplateLevel::TemplateLevel(std::string  file) : dir(DIR), filename(file), ext(LEVELLOADERFILETYPE), TileSize(100, 100), grid(0, 0)
{
  LevelIO(file, dir);
}

/**
 * \brief 
 * \param file 
 * \param newdir 
 */
TemplateLevel::TemplateLevel(std::string file, std::string newdir) : dir(DIR), filename(file), ext(LEVELLOADERFILETYPE), TileSize(100, 100), grid(0, 0)
{
  LevelIO(file, newdir);
}


/**
 * \brief 
 */
TemplateLevel::~TemplateLevel()
{
}

void TemplateLevel::SetPos(float x, float y)
{
  grid.x = x;
  grid.y = y;
}

/**
 * \brief 
 * \param x 
 * \param y 
 */
void TemplateLevel::SetScale(float x, float y)
{
  TileSize.x = x;
  TileSize.y = y;
}

//void TemplateLevel::construct_ui(std::vector<GameObject>& man)
//{
  /*
  Json::Value obj;
  Json::Value obj1;
  Json::Value obj2;

  Read_Buffer("data/json/Life.json", &obj);
  Read_Buffer("data/json/HighScore.json", &obj1);
  Read_Buffer("data/json/Score.json", &obj2);


   GameObject life = ObjectFactory::DeSerializeArchetype(obj["Object"][0]);
   GameObject score = ObjectFactory::DeSerializeArchetype(obj1["Object"][0]);
   GameObject hscore = ObjectFactory::DeSerializeArchetype(obj2["Object"][0]);

  man.push_back(std::move(life));
  man.push_back(std::move(score));
  man.push_back(std::move(hscore));*/
  //}

/**
 * \brief 
 * \param ss 
 */
void TemplateLevel::getkey(std::stringstream& ss)
{
  ss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//jump to newline
  //ss.ignore(1);//1 past newline
  char input;
  ss >> input;
  while (input != ',')
  {
    ss.ignore(1);
    std::string str;
    std::string bl;
    std::getline(ss, str, ',');
    std::getline(ss, bl);

    bool boolcheck = stringtobool(bl);

    Tile2Archetype.emplace(input, std::make_pair(dir + str + ARCHETYPEFILETYPE, boolcheck));
    ss >> input;
  }
  ss.ignore(2); // next line
}

/**
 * \brief 
 * \param ss 
 */
void TemplateLevel::getmap(std::stringstream& ss)
{
  ss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//jump to newline
  while (ss.peek() != ',')
  {
    std::string str;
    std::getline(ss, str);
    map.push_back(str);
  }

}

/**
 * \brief 
 * \param bl 
 * \return 
 */
bool TemplateLevel::stringtobool(const std::string& bl)
{
  if (!bl.empty()) {
    ;
    return (std::tolower(bl[0]) == 't');
  }
  else
    return false;
}

/**
 * \brief 
 * \param b 
 * \param x 
 * \param y 
 * \param game_object 
 */
void printTile(bool b, float x, float y, const GameObject& game_object)
{
  if (b)
  {
    engine.Find<Trace::TraceHandler>()->TraceMessage(TraceLevel::INFO, ("|" + std::to_string(x) + ": " + game_object.GetType() + " :" + std::to_string(y) + "|").c_str());
  }
}

/**
 * \brief 
 */
void printdeadspace()
{
  engine.Find<Trace::TraceHandler>()->TraceMessage(TraceLevel::INFO, "...dead tile..");
}

/**
 * \brief 
 * \param man 
 */
void TemplateLevel::constructlevel(std::vector<GameObject>& man)
{
  if (!isOpen) return;
  auto findLongestRow =
    [](const std::vector<std::string>&  map)
  {
    unsigned int longest = 0;
    for (unsigned int i = 0; i < map.size(); ++i)
    {
      if (longest < map[i].length()) longest = map[i].length();
    }
    return longest;
  };

  typedef std::vector<std::reference_wrapper<GameObject>> Objects;
  unsigned int width = findLongestRow(map);


  for (unsigned int i = 0; i < map.size(); ++i)
  {
    for (unsigned int j = 0; j < width;++j)
    {
      const auto findresult = Tile2Archetype.find(map[i][j]);

      if (findresult == Tile2Archetype.end())
      {
        //printdeadspace();
        continue;
      }

      //float x = ( (-0.5f * TileSize.x * width) +  (static_cast<float>(i) / width) ) * TileSize.x;
      //float y = ( (0.5f *  TileSize.y  * height) + ( static_cast<float>(j) / height) ) * TileSize.y;
      auto findrange = [&]
      {
        const int found = map[i].find_first_not_of(findresult->first, j); // find the next non-wall title
        return ((found != std::string::npos) ? found : width) - j - 1;    // use the width of the map if fell out of the string
      };

      const int range = (findresult->second.second) // if the tile allows combining
        ? findrange()                               // find the range
        : 0;                                        // sub in 0

      GameObject obj =
        ObjectFactory::ArchetypeGameObject(
          findresult->second.first.c_str(),
          glm::vec2 {
            (j + 0.5f * range) * TileSize.x + grid.x,
            -((i * TileSize.y) + grid.y)
          }
        );

      Transform * t = obj.Find<Transform>();
      t->setXScale(t->getXScale() * (range + 1));

      //printTile(true, x, y, obj);

      man.emplace_back(std::move(obj));
      j += range;
    }
    //engine.Find<Trace::TraceHandler>()->TraceMessage(TraceLevel::INFO, "\n");
  }
}

/**
 * \brief 
 * \param file 
 * \param _dir 
 */
void TemplateLevel::LevelIO(std::string file, std::string _dir)
{
  auto readfile = [&]
  {
    // open file for input, start at the very end of the file
    std::ifstream infile{ std::string{_dir + filename + ext}, std::ios_base::in | std::ios_base::ate };
    
    // check if open
    if (!infile)
    {
      isOpen = false;
      return std::move(std::string{});
    }
    isOpen = true;

    /// read file ///
    std::string buffer{};
    buffer.resize(static_cast<size_t>(infile.tellg())); // get the position into the file in bytes
    infile.seekg(0, std::ios::beg);                     // seek back to the beginning of the file
    infile.read(&buffer[0], buffer.size());               // read entire file into string
    
    infile.close(); // close file

    return std::move(buffer);
  };

  std::stringstream ss{ readfile() };
  if (!isOpen)
  {
    std::cout << "cannot open file\n";
    return;
  }
  getkey(ss); //read in key
  getmap(ss); //read in objects
}
