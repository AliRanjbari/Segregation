# Schelling's Model of Segregation

it's my implementation of [Schelling's](https://en.wikipedia.org/wiki/Thomas_Schelling) model with Cpp language

You can see a review of his model [here](http://nifty.stanford.edu/2014/mccown-schelling-model-segregation/)


## Usage

* first compile .cpp file with g++ compiler
* make a map file (like the sample map.txt)
  * R -> Red
  * B -> Blue
  * E -> Empty
* now use : ./a.out -f <map name> -s <number> -p <number>
  * -f -> your map
  * -s -> number of iteration (if provided code will run until everyone is satisfied)
  * -p -> similarity percentile (if not provided if will be 30)
  
  
  ## Output
  
  program will output in terminal first the number of unhappy persons the the final map  
  and a .ppm image of map will be created
