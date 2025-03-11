#define         UNDEFIND       -1
#define         FALSE           0
#define         TRUE            1
#define         FALSE1          2
#define         FALSE2          3

#define         PRIORONE        1
#define         PRIORTWO        2
#define         PRIORTHR        3
#define         PRIORFOR        4
#define         PRIORFVE        5
#define         PRIORSIX        6

#define         COUNT           30
#define         HOMEKEY         71
#define         ENDKEY          79
#define         ESCKEY          27
#define         LEFTKEY         75
#define         RIGHTKEY        77
#define         UPKEY           72
#define         DOWNKEY         80
#define         COMMENTS        92

#define         MAXSTR          19
#define         MINTOKEN        49
#define         MAXTOKEN        498
#define         MAXTOKENO       499
#define         MAXINPUT        149
#define         MAXSIZE         8190

#define         NUMBER          10
#define         VARIABLE        11
#define         OPERATOR        12
#define         FUNCTION        13
#define         CONFLOWS        14
#define         COMMANDS        15
#define         JUSTGOTO        16
#define         COMPGOTO        17
#define         PROCEDUR        18
#define         IO_PARTS        19

#define         SIN             100
#define         COS             101
#define         TAN             102
#define         SINH            103
#define         COSH            104
#define         TANH            105
#define         ASIN            106
#define         ACOS            107
#define         ATAN            108
#define         ABS             109
#define         INT             110
#define         SQRT            111
#define         LOG             112
#define         LOG10           113
#define         EXP             114
#define         QSORT           115
#define         POWER           116
#define         RAND            117
#define         RAND1           118
#define         RAND2           119
#define         DEMO            120
#define         EXPDEV          121
#define         NOMDEV          122
#define         GAMDEV          123
#define         POIDEV          124
#define         BINDEV          125
#define         MATRIX          126
#define         IDENTITY        127
#define         HILBERT         128
#define         LUD             129
#define         INV             130
#define         DET             131
#define         RANK            132
#define         UPDATE          133
#define         DIM             134
#define         MMUL            135
#define         SVD             136
#define         EIG             137
#define         TRANS           138
#define         CHOL            139
#define         HOUS            140
#define         PLUS            141
#define         MINUS           142
#define         MUL             143
#define         DIV             144
#define         FORMAT          145
#define         VNAME           146
#define         QLD             147
#define         QUIT            148
#define         SHELL           149
#define         CLS             150
#define         VLIST           151
#define         HELP            152
#define         EDIT            153
#define         MEMO            154
#define         PLIST           155
#define         DIR             156
#define         TYPE            157
#define         PAUSE           158
#define         MAX             159
#define         MIN             160
#define         MEAN            161
#define         COV             162
#define         CORR            163
#define         DSUM            164
#define         DPROD           165
#define         INCLUDE         166
#define         INPUT           167
#define         OUTPUT          168
#define         QUESTION        169
#define         GAMMA           170
#define         FACTRL          171
#define         BINOMIAL        172
#define         BETA            173
#define         BESSJ           174
#define         BESSY           175
#define         ONEPLOT         176
#define         TWOPLOT         177
#define         THRPLOT         178
#define         TWOBAR          179
#define         TWOPIE          180
#define         XYPLOT          181
#define			NOHELP			182
#define         VFREE           183
#define         PFREE           184
#define         CTRLC           185
#define         EQUALS          186
#define         SEMICOL         187
#define         COMMA           188
#define         LIST            189
#define         TLIST           190
#define         LESS            191
#define         GREATER         192
#define         GREATEREQUAL    193
#define         LESSEQUAL       194
#define         NOTEQUAL        195
#define         FOR             196
#define         ENDFOR          197
#define         IF              198
#define         ELSEIF          199
#define         ELSE            200
#define         ENDIF           201
#define         DEF             202
#define         ENDDEF          203
#define         BAR             204
#define         AMPER           205
#define         EXCLAM          206
#define         LPAREN          207
#define         RPAREN          208
#define         LBRACK          209
#define         RBRACK          210
#define         LBRACE          211
#define         RBRACE          212
#define         COLON           213
#define         DQOUTE          214
#define			INFO			215
#define         PRINT           216



#define         ESILON          1E-20

struct token {
    char    name[MAXSTR];
    int     type;
    int     prior;
};

struct procedur {
    char    name[MAXSTR];
    int     count;
    struct  token *start;
};

struct variable {
    char    name[MAXSTR];
	char	*label;
    int     row;
    int     column;
    double  *start;
};


#if defined(TOKEN)

#define     NIO_PARTS       5
struct io_parts {
    char    *name;
    int     prior;
} lio_parts[] = {
    "include",  INCLUDE,
    "input",    INPUT,
    "output",   OUTPUT,
    "type",     TYPE,
    "print",    PRINT
};

#define     NCOMMANDS       19
struct commands {
    char    *name;
    int     prior;
} lcommands[] = {
    "memory",   MEMO,
    "quit",     QUIT,
    "exit",     QUIT,
    "bye",      QUIT,
    "end",      QUIT,
    "rand",     RAND,
    "exelist",  LIST,
    "toklist",  TLIST,
    "shell",    SHELL,
    "cls",      CLS,
    "help",     HELP,
    "vlist",    VLIST,
    "plist",    PLIST,
    "pause",    PAUSE,
    "dir",      DIR,
	"edit",     EDIT,
	"demo",     DEMO,
	"Nohelp",	NOHELP,
	"info",		INFO
};

#define     NFUNCTION       69
struct function {
    char    *name;
    int     prior;
} lfunction[] = {
    "+",        PRIORONE,
    "-",        PRIORONE,
    "*",        PRIORTWO,
    "#",        PRIORTWO,
    "/",        PRIORTWO,
    "^",        PRIORTHR,
    "~",        PRIORTHR,
    "`",        PRIORTHR,
    "exp",      PRIORTHR,
    "det",      PRIORTHR,
    "rank",     PRIORTHR,
    "inv",      PRIORTHR,
    "dim",      PRIORTHR,
    "svd",      PRIORTHR,
    "abs",      PRIORTHR,
    "cos",      PRIORTHR,
    "sin",      PRIORTHR,
    "tan",      PRIORTHR,
    "cosh",     PRIORTHR,
    "sinh",     PRIORTHR,
    "tanh",     PRIORTHR,
    "log",      PRIORTHR,
    "int",      PRIORTHR,
    "acos",     PRIORTHR,
    "asin",     PRIORTHR,
    "atan",     PRIORTHR,
    "sqrt",     PRIORTHR,
    "log10",    PRIORTHR,
    "house",    PRIORTHR,
    "expdev",   PRIORTHR,
    "nomdev",   PRIORTHR,
    "gamdev",   PRIORTHR,
    "poidev",   PRIORTHR,
    "bnldev",   PRIORTHR,
    "gamma",    PRIORTHR,
    "factrl",   PRIORTHR,
    "bico",     PRIORTHR,
    "beta",     PRIORTHR,
    "bessJ",    PRIORTHR,
    "bessY",    PRIORTHR,
    "chol",     PRIORTHR,
    "rand1",    PRIORTHR,
    "rand2",    PRIORTHR,
    "QL",       PRIORTHR,
    "LU",       PRIORTHR,
    "eig",      PRIORTHR,
    "max",      PRIORTHR,
    "min",      PRIORTHR,
    "mean",     PRIORTHR,
    "cov",      PRIORTHR,
    "corr",     PRIORTHR,
    "dsum",     PRIORTHR,
    "dprod",    PRIORTHR,
    "qsort",    PRIORFOR,
    "2pie",     PRIORFOR,
    "1plot",    PRIORFOR,
    "2plot",    PRIORFOR,
    "3plot",    PRIORFOR,
    "2bar",     PRIORFOR,
    "3bar",     PRIORFOR,
    "xyplot",   PRIORFOR,
    "format",   PRIORFOR,
    "matrix",   PRIORFOR,
    "identity", PRIORFOR,
    "hilbert",  PRIORFOR,
    "Vname",    PRIORFOR,
    "vfree",    PRIORFOR,
    "pfree",    PRIORFOR,
    "return",   PRIORFVE
};

#define     NOPERATOR       17
struct operator {
    char    *name;
    int     prior;
} loperator[] = {
    "(",        LPAREN,
    ")",        RPAREN,
    "[",        LBRACK,
    "]",        RBRACK,
    "=",        EQUALS,
    ":",        COLON,
    ";",        SEMICOL,
    ",",        COMMA,
    "\"",       DQOUTE,
    "|",        BAR,
    "&",        AMPER,
    "!",        EXCLAM,
    "<",        LESS,
    ">",        GREATER,
    "?",        QUESTION,
    "{",        LBRACE,
    "}",        RBRACE
};

#define     NCONFLOWS       8
struct conflows {
    char    *name;
    int     prior;
} lconflows[] = {
    "for",      FOR,
    "endfor",   ENDFOR,
    "if",       IF,
    "else",     ELSE,
    "elseif",   ELSEIF,
    "endif",    ENDIF,
    "def",      DEF,
    "enddef",   ENDDEF
};

#define     NCONCHARS       25
struct conchars {
    char    *name;
} lconchars[] = {
    "(",")","+","-","[","]","/","*","#","=","'",
    ";","^",",","<",">","\\","{","}","?","~","`",":","\"","!"
};
#endif


#define     Error         999
#define     Systerror0    1000
#define     Systerror1    1001
#define     Systerror2    1002
#define     Systerror3    1003
#define     Systerror4    1004
#define     Systerror5    1005
#define     Systerror6    1006
#define     Systerror7    1007
#define     Systerror8    1008
#define     Systerror9    1009
#define     Systerror10   1010
#define     Systerror11   1011
#define     Systerror12   1012
#define     Systerror13   1013
#define     Systerror14   1014
#define     Systerror15   1015
#define     Systerror16   1016
#define     Systerror17   1017
#define     Systerror18   1018
#define     Systerror19   1019
#define     Matherror0    1020
#define     Matherror1    1021
#define     Matherror2    1022
#define     Matherror3    1023
#define     Matherror4    1024
#define     Matherror5    1025
#define     Matherror6    1026
#define     Matherror7    1027
#define     Matherror8    1028
#define     Matherror9    1029
#define     Matherror10   1030
#define     Matherror11   1031
#define     Matherror12   1032
#define     Matherror13   1033
#define     Matherror14   1034
#define     Matherror15   1035
#define     Matherror16   1036
#define     Matherror17   1037
#define     Matherror18   1038
#define     Matherror19   1039
#define     Matherror20   1040
#define     Matherror21   1041
#define     Matherror22   1042
#define     Matherror23   1043
#define     Matherror24   1044
#define     Matherror25   1045
#define     Matherror26   1046
#define     Matherror27   1047
#define     Matherror28   1048
#define     Matherror29   1049
#define     Matherror30   1050
#define     Matherror31   1051
#define     Matherror32   1052
#define     Matherror33   1053
#define     Matherror34   1054
#define     Matherror35   1055
#define     Matherror36   1056
#define     Matherror37   1057
#define     Matherror38   1058
#define     Matherror39   1059
#define     Matherror40   1060
#define     Matherror41   1061
#define     Matherror42   1062
#define     Matherror43   1063
#define     Matherror44   1064
#define     Matherror45   1065
#define     Matherror46   1066
#define     Matherror47   1067
#define     Matherror48   1068
#define     Matherror49   1069
#define     Matherror50   1070
#define     Matherror51   1071
#define     Matherror52   1072
#define     Matherror53   1073
#define     Matherror54   1074
#define     Matherror55   1075
#define     Matherror56   1076
#define     Matherror57   1077
#define     Matherror58   1078
#define     Matherror59   1079
#define     Matherror60   1080
#define     Matherror61   1081
#define     Matherror62   1082
#define     Matherror63   1083
#define     Matherror64   1084
#define     Matherror65   1085
#define     Matherror66   1086
#define     Matherror67   1087
#define     Matherror68   1088
#define     Matherror69   1089
#define     Matherror70   1090
#define     Matherror71   1091
#define     Matherror72   1092
#define     Matherror73   1093
#define     Matherror74   1094
#define     Matherror75   1095
#define     Matherror76   1096
#define     Matherror77   1097
#define     Matherror78   1098
#define     Matherror79   1099
#define     Matherror80   1100
#define     Matherror81   1101
#define     Matherror82   1102
#define     Matherror83   1103
#define     Matherror84   1104
#define     Matherror85   1105
#define     Matherror86   1106
#define     Matherror87   1107
#define     Matherror88   1108
#define     Matherror89   1109
#define     Matherror90   1110
#define     Matherror91   1111
#define     Matherror92   1112
#define     Matherror93   1113
#define     Matherror94   1114
#define     Matherror95   1115
#define     Matherror96   1116
#define     Matherror97   1117
#define     Matherror98   1118
#define     Matherror99   1119
#define     Matherror100  1120
#define     Matherror101  1121
#define     Matherror102  1122
#define     Matherror103  1123
#define     Matherror104  1124
#define     Matherror105  1125
#define     Matherror106  1126
#define     Matherror107  1127
#define     Matherror108  1128
#define     Matherror109  1129
#define     Matherror110  1130
#define     Matherror111  1131
#define     Matherror112  1132
#define     Matherror113  1133
#define     Matherror114  1134
#define     Matherror115  1135
#define     Matherror116  1136
#define     Matherror117  1137
#define     Matherror118  1138
#define     Matherror119  1139
#define     Matherror120  1140
#define     Matherror121  1141
#define     Matherror122  1142
#define     Matherror123  1143
#define     Matherror124  1144
#define     Matherror125  1145
#define     Matherror126  1146
#define     Matherror127  1147
#define     Matherror128  1148
#define     Matherror129  1149
#define     Matherror130  1150
#define     Matherror131  1151
#define     Matherror132  1152
#define     Matherror133  1153
#define     Matherror134  1154
#define     Matherror135  1155
#define     Matherror136  1156
#define     Matherror137  1157
#define     Matherror138  1158
#define     Matherror139  1159
#define     Matherror140  1160
#define     Matherror141  1161
#define     Matherror142  1162
#define     Matherror143  1163
#define     Matherror144  1164
