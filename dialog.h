// MAX LENGHT IS 23
#define DIALOG_LENGTH 24
#define STORYLINE_LENGTH 
// Empty 23 char long string
const unsigned char empty_line[] = "                       ";

const unsigned char story[][] = {
    "The city of Xyleria was ruled by Kharzoid, a dark and powerful wizard.",
    "In his twisted imagination, he conceived a diabolical game, filled with traps and foes.",
    "Each year, a prisoner would get a chance of redemption if he could succeed the trial.",
    "His soul was transfered into a magical skull. If it got destroyed, his soul was lost.",
    "But a brave and courageous man stood up to the challenge.",
    "He got put in jail, although he was faultless. And waited patiently for the day of the Trial.",
    "The day of revenge."

};

const unsigned char dialogs[][DIALOG_LENGTH] = {
    // CEMETERY
    "Where... ",    // 0
    "Where am I?",  // 1
    // ANGELICA
    "Do not be afraid.",       // 2
    "I was sent to help you",  // MAX!

    "You are about to enter",  // 4
    "the Trial of Kharzoid!",  // 5

    "You must go back to",  // 6
    "the castle, and defeat",
    "this evil necromant",

    "Take this!",  // 9

    "Use it if you get stuck",  // 10
    "Just press the B button",

    "Hit all the tombstones",  // 12
    "to get out of here",
    "We'll meet again soon!",

};
