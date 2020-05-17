#include <iostream>
#include <string>
#include "WordLibrary.h"
#include <cstdlib>
#include <ctime>

using namespace std;

WordLibrary :: WordLibrary(){
        
        //What is the better design between adding each word in class like below, or loading from file.

        library[0].word = "Avocado";
        library[0].hint = "Main ingredient in guacamole";
        library[1].word = "Broccoli";
        library[1].hint = "King of vegetables, kids hate it";
        library[2].word = "Carrot";
        library[2].hint = "Rabbit's favourite";
        library[3].word = "Eggplant";
        library[3].hint = "Long vegetable, purple on the outside but white on the inside";
        library[4].word = "Grapefruit";
        library[4].hint = "Tastse like an orange, looks like an orange, but bigger";
        library[5].word = "Kiwi";
        library[5].hint = "It's a fruit covered with small hairs, green inside, tastes sour";
        library[6].word = "Lemon";
        library[6].hint = "Yellow and sour, people like to juice them";
        library[7].word = "Mango";
        library[7].hint = "Tropical fruit, yellow and sweet";
        library[8].word = "Onion";
        library[8].hint = "It makes you cry when you cut it";
        library[9].word = "Potato";
        library[9].hint = "Good cabohydrate plant";
        library[10].word = "Raisin";
        library[10].hint = "Dry grape";
        library[11].word = "Tomato";
        library[11].hint = "It's red, it's juicy, it makes your skin bright";
        library[12].word = "Watermelon";
        library[12].hint = "It's red but covered in green, great for summer";
        library[13].word = "Zucchini";
        library[13].hint = "You may confuse it with a cucumber, but it's not";
        library[14].word = "Argentina";
        library[14].hint = "Capital city: Buenos Aires";
        library[15].word = "Brazil";
        library[15].hint = "One of the well known countries for soccer";
        library[16].word = "Canada";
        library[16].hint = "You may find the greatest maple syrup in this country";
        library[17].word = "Egypt";
        library[17].hint = "Well known for mummies";
        library[18].word = "France";
        library[18].hint = "World of perfume";
        library[19].word = "Guam";
        library[19].hint = "It's an island in United State territory";
        library[20].word = "Coronavirus";
        library[20].hint = "What makes everyone stay home in 2020";
        library[21].word = "Iceland";
        library[21].hint = "This country sounds cold, but is full of green";
        library[22].word = "Japan";
        library[22].hint = "2020 Olympics host";
        library[23].word = "Kenya";
        library[23].hint = "World's fastest men alive come from there";
        library[24].word = "Australia";
        library[24].hint = "Land of the kangaroos";
        library[25].word = "Russia";
        library[25].hint = "Land of the best vodka";
        library[26].word = "Spain";
        library[26].hint = "World famous matadors";
        library[27].word = "Thailand";
        library[27].hint = "The land of smile";
        library[28].word = "Scotland";
        library[28].hint = "The land of kilts and tartan";
        library[29].word = "Alligator";
        library[29].hint = "Relative to crocodile";
        library[30].word = "Bear";
        library[30].hint = "His favourite is honey";
        library[31].word = "Camel";
        library[31].hint = "Great vehicle in the desert";
        library[32].word = "Dog";
        library[32].hint = "The most loyal creature";
        library[33].word = "Elephant";
        library[33].hint = "They are big with a long nose";
        library[34].word = "Falcon";
        library[34].hint = "You may confuse it with a hawk or an eagle";
        library[35].word = "Hyena";
        library[35].hint = "Laughter animal";

        shuffle();
 
}

string WordLibrary::getWord(int index){
    return library[index].word;
}

string WordLibrary::getHint(int index){
    return library[index].hint;
}


void WordLibrary :: shuffle(){

    string wordTemp;
    string hintTemp;
    int index;
    srand(time(NULL));

    for (int i = 0; i < COUNT; i++)
    {
        index = rand() % COUNT;
        wordTemp = library[i].word;
        hintTemp = library[i].hint;

        library[i].word = library[index].word;
        library[i].hint = library[index].hint;

        library[index].word = wordTemp;
        library[index].hint = hintTemp;

    }


}

