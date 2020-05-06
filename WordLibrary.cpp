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
        library[1].hint = "King of vegetable, kids hate it";
        library[2].word = "Carrot";
        library[2].hint = "Rabbit's favourite";
        library[3].word = "Eggplant";
        library[3].hint = "Long shape vegetable, purple outside but white inside";
        library[4].word = "Grapefruit";
        library[4].hint = "Taste like orange, look like orange, but bigger";
        library[5].word = "Kiwi";
        library[5].hint = "It's a fruit cover with small hair, green inside, taste sour";
        library[6].word = "Lemon";
        library[6].hint = "Yellow and sour, people like to make them juice";
        library[7].word = "Mango";
        library[7].hint = "Tropical fruit, yellow and sweet";
        library[8].word = "Onion";
        library[8].hint = "It makes you cry when you cut it";
        library[9].word = "Potato";
        library[9].hint = "Good cabohydrate plant";
        library[10].word = "Raisin";
        library[10].hint = "Dry grape";
        library[11].word = "Tomato";
        library[11].hint = "It's red, it's juicy, it's make your skin bright";
        library[12].word = "Watermelon";
        library[12].hint = "It's red but cover in green, great for summer";
        library[13].word = "Zucchini";
        library[13].hint = "You may confuse it with cucumber, but it's not";
        library[14].word = "Argentina";
        library[14].hint = "Capital city: Buenos Aires";
        library[15].word = "Brazil";
        library[15].hint = "One of the well known country for soccer";
        library[16].word = "Canada";
        library[16].hint = "You may find the greatest maple syrup in this country";
        library[17].word = "Egypt";
        library[17].hint = "Well known for mummy";
        library[18].word = "France";
        library[18].hint = "World of perfume";
        library[19].word = "Guam";
        library[19].hint = "It's an island in United State territory";
        library[20].word = "Hong Kong";
        library[20].hint = "It's an island in China territory";
        library[21].word = "Iceland";
        library[21].hint = "It's sound cold, but full of green";
        library[22].word = "Japan";
        library[22].hint = "2020 Olympic's host";
        library[23].word = "Kenya";
        library[23].hint = "World fastest man alive";
        library[24].word = "New Zealand";
        library[24].hint = "Homeland of Maori";
        library[25].word = "Russia";
        library[25].hint = "Most famous vodka ever";
        library[27].word = "Thailand";
        library[27].hint = "Land of smile";
        library[28].word = "United Kingdom";
        library[28].hint = "You may call Great Britain";
        library[29].word = "Alligator";
        library[29].hint = "Relative to crocodile";
        library[30].word = "Bear";
        library[30].hint = "His favourite is honey";
        library[31].word = "Camel";
        library[31].hint = "Great vehicle in dessert";
        library[32].word = "Dog";
        library[32].hint = "The most loyalty creature";
        library[33].word = "Elephant";
        library[33].hint = "They are big with long long nose";
        library[34].word = "Falcon";
        library[34].hint = "You may confuse with hawk or eagle";
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

