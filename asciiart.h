#pragma once
const char* map =
"7-----------------------0---------------------------------1  Time: \n"
"| + . . . . . . . . . . + . . . . . . . . . . . . . . . + |  Safe: \n"
"| . + . . . . . . . . . + . . . . . . . . . . . . . . + . |  Plane:   Destination:\n"
"| . . + . . . . . . . . + . . . . . . . . . . . . . + . . |\n"
"| . . . + . . . . . . . + . . . . . . . . . . . . + . . . |\n"
"| . . . . + . . . . . . + . . . . . . . . . . . + . . . . |\n"
"| . . . . . + . . . . . + . . . . . . . . . . + . . . . . |\n"
"6 + + + + + + + + + + + *0+ + + + + + + + + + + + + + + + 2\n"
"| . . . . . . . . . . . + . . . . . . . . + . . . . . . . |\n"
"| . . . . . . . . . . . + . . . . . . . + . . . . . . . . |\n"
"| . . . . . . . . . . . + . . . . . . + . . . . . . . . . |\n"
"| . . . . . . . . . . . + . . . . . + . . . . . . . . . . |\n"
"| . . . . . . . . . . . + . . . . + . . . . . . . . . . . |\n"
"5 + + + + + + + + + + + + . . . + . . . . . . . . . . . . |\n"
"| . . . . . . . . . . . + . . + . . . . . . . . . . . . . |\n"
"| . . . . . . . . . . . + . + . . . . . ^0. . . . . . . . |\n"
"| . . . . . . . . . . . + + . . . . . . . . . . . . . . . |\n"
"| . . . . . . . . . . . *1+ + + + + + + + + + + + + + + + 3  /restart\n"
"| . . . . . . . . . . + . . . . . . . . >1. . . . . . . . |  /menu\n"
"| . . . . . . . . . + . . . . . . . . . . . . . . . . . . |  /exit\n"
"------------------4----------------------------------------\n"
"command: ";

const char* menu =
"\n"
"___________________________________\n"
"_______________    |__  __/_  ____/\n"
"______________  /| |_  / __  /     \n"
"_____________  ___ |  /   / /___\n"
"____________/_/  |_/_/    \\____/       _  _\n"
"                                      ( `   )_\n"
"                                     (    )    `)\n"
"                          _         (_   (_ .  _) _)\n"
"                       -=\\`\\\n"
"                   |\\ ____\\_\\__                                     type\n"
"                 -=\c`\"\"\"\"\"\"\" \" `)                                   /play\n"
"                    `~~~~~/ /~~`                                    /scoreboard\n"
"                      -==/ /                                        /exit\n"
"                        '-'                          _\n"
"                                                    (  )\n"
"          _ .                                    ( `  ) . )\n"
"        (  _ )_                                 (_, _(  ,_)_)\n"
"      (_  _(_ ,)\n"
"\n"
"_____________________________________________________________________________________\n"
"command: \n"
"                                                          PASSAMON SUKMAKSIN 64011199" ;

const char* scoreboard =
"\n"
"    SCOREBOARD\n"
"\n"
"    1.                                                |\n"
"                                                --====|====--\n"
"    2.                                                |\n"
"\n"
"    3.                                            .-\"\"\"\"\"-.\n" 
"                                                .\'_________\'.\n"
"    4.                                         /_/_|__|__|_\\_\\\n"
"                                              ;\'-._       _.-\';\n"
"    5.                   ,--------------------|    \`-. .-\'    |--------------------,\n"
"                          ``\"\"--..__    ___   ;       \'       ;   ___    __..--\"\"\`\`\n"
"    6.                             `\" -// \\\\.._\\             /_..// \\\\-\"\`\n"
"                                       \\\\_//    \'._       _.\'    \\\\_//\n"
"    7.                                  \`\"\`        \`\`---\`\`        \`\"\`\n"
"\n"
"    8.\n"
"\n"
"    9.                                                                   /menu\n"
"_____________________________________________________________________________________\n"
"command: ";

const char* loseMap =
"7-----------------------0---------------------------------1  Time: \n"
"| + . . . . . . . . . . + . . . . . . . . . . . . . . . + |  Safe: \n"
"| . + . . . . . . . . . + . . . . . . . . . . . . . . + . |  Plane:   Destination:\n"
"| . . + . . . . . . . . + . . . . . . . . . . . . . + . . |\n"
"| . . . + . . . . . . . + . . . . . . . . . . . . + . . . |\n"
"| . . . . + . . . . . . + . . . . . . . . . . . + . . . . |\n"
"| . . . . . + . . . . . + . . . . . . . . . . + . . . . . |\n"
"6 + + + + + + + + + + + *0+ + + + + + + + + + + + + + + + 2\n"
"| . . . . . . . . . . . + . . . . . . . . + . . . . . . . |\n"
"| . . . . . . . . . . . + . . . . . . . + . . . . . . . . |\n"
"| . . . . . . . . . . . + . . . . . . + . . . . . . . . . |\n"
"| . . . . . . . . . . . + . . . . . + . . . . . . . . . . |\n"
"| . . . . . . . . . . . + . . . . + . . . . . . . . . . . |  to save your score\n"
"5 + + + + + + + + + + + + . . . + . . . . . . . . . . . . |  type\n"
"| . . . . . . . . . . . + . . + . . . . . . . . . . . . . |  /save [name]\n"
"| . . . . . . . . . . . + . + . . . . . ^0. . . . . . . . |  (name must be between\n"
"| . . . . . . . . . . . + + . . . . . . . . . . . . . . . |  1-14 characters)\n"
"| . . . . . . . . . . . *1+ + + + + + + + + + + + + + + + 3  /restart\n"
"| . . . . . . . . . . + . . . . . . . . >1. . . . . . . . |  /menu\n"
"| . . . . . . . . . + . . . . . . . . . . . . . . . . . . |  /exit\n"
"------------------4----------------------------------------\n"
"command: \n"
"GAME OVER!";
