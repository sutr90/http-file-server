#include "names.h"

const std::string names::animals[] = {"Ant", "Ape", "Donkey", "Baboon", "Badger", "Bat", "Bear", "Beaver", "Bee",
                                           "Camel", "Caribou", "Cat", "Cicada", "Clam", "Cockroach", "Cod", "Coyote",
                                           "Crab", "Cricket", "Crow", "Deer", "Dog", "Dolphin", "Duck", "Elk", "Fish",
                                           "Fly", "Fox", "Frog", "Gerbil", "Goat", "Hamster", "Hare", "Hornet", "Horse",
                                           "Hound", "Hyena", "Impala", "Jackal", "Koala", "Leopard", "Lion", "Lizard",
                                           "Locust", "Louse", "Mole", "Monkey", "Mouse", "Rat", "Mule", "Ox", "Oyster",
                                           "Panda", "Pig", "Platypus", "Porcupine", "Salmon", "Sardine", "Scorpion",
                                           "Seal", "Shark", "Sheep", "Skunk", "Snail", "Snake", "Spider", "Termite",
                                           "Tiger", "Walrus", "Wasp", "Whale", "Wolf", "Wombat", "Worm", "Yak",
                                           "Zebra"};

const std::string names::adjectives[] = {"Anal", "Baked", "Barbed", "Bare", "Bent", "Birch", "Bland", "Blond",
                                              "Blunt", "Boiled", "Bold", "Boned", "Brave", "Brief", "Brisk", "Broke",
                                              "Brute", "Burned", "Calm", "Cheap", "Chill", "Clean", "Cool", "Cracked",
                                              "Cramped", "Crank", "Crisp", "Cute", "Daft", "Damp", "Dank", "Deaf",
                                              "Dense", "Dire", "Dried", "Drunk", "Dull", "Faint", "Fake", "Fat", "Firm",
                                              "Fit", "Fold", "Fresh", "Fun", "Ghast", "Glazed", "Glum", "Grave",
                                              "Grown", "Huge", "Hurt", "Iced", "Jade", "Kind", "Lame", "Lean", "Left",
                                              "Limp", "Loud", "Mad", "Masked", "Metal", "Mild", "Moist", "Mute", "Neat",
                                              "Nude", "Numb", "Pale", "Plump", "Proud", "Punk", "Queer", "Rare", "Sad",
                                              "Shaped", "Shy", "Slate", "Sly", "Smug", "Sore", "Spare", "Sport",
                                              "Stale", "Steep", "Stiff", "Stray", "Strict", "Stuck", "Stung", "Super",
                                              "Tame", "Tan", "Teen", "Tired", "Torn", "Twin", "Wide", "Wired", "Worn"};

std::string names::get_random_name() {

    unsigned char idx_animal = names::rng.get_random_8bit_number() % names::ANIMAL_COUNT;
    unsigned char idx_adjective = names::rng.get_random_8bit_number() % names::ADJECTIVE_COUNT;
    return names::adjectives[idx_adjective] + names::animals[idx_animal];
}

dlib::rand names::rng(time(NULL));