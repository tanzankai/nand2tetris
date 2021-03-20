#include<string.h>
#include<errno.h> // エラーハンドリング
#include<ctype.h>
#include<dirent.h>
#include "fileoperation.h"
#include "tokenizer.h"
#include "codegen.h"

void process(char *jackfilepath, char *xmlfilepath){
    FILE *fp_jack, *fp_xml;
    if((fp_jack = fopen(jackfilepath, "r")) == NULL)
        error("Error: cannot open jackfile.");

    if((fp_xml = fopen(xmlfilepath, "w")) == NULL)
        error("Error: cannot open xmlfile.");
    
    JackTokenizer *tokenizer = init(fp_jack);
    codegen(fp_xml, tokenizer);
}

int main(int argc, char *argv[]){
    DIR *dp;
    if(argc != 2) error("Error: the number of parameters must be 2.");

    dp = opendir(argv[1]);

    // ディレクトリだった場合
    if(dp){
        struct dirent *entry;
        char *dirname = argv[1];
        char jackfilepath[100];
        char xmlfilepath[100];

        while((entry = readdir(dp)) != NULL){
            char *filename = entry->d_name;
            if(!isJackfile(filename)) continue;

            // jackファイルへの相対パスを生成 (オーバーフロー対策しろや)
            strcpy(jackfilepath, dirname);
            strcat(jackfilepath, "/");
            strcat(jackfilepath, filename);

            // 出力先のxmlファイルの生成 (オーバーフロー対策しろや)
            strcpy(xmlfilepath, dirname);
            strcat(xmlfilepath, "/");
            strncat(xmlfilepath, filename, strlen(filename) - strlen(".jack"));
            strcat(xmlfilepath, "T.xml");

            process(jackfilepath, xmlfilepath);
        }
    }
    // ディレクトリではなくファイルだった場合
    else if(errno == ENOTDIR && isJackfile(argv[1])){
        char *filename = argv[1];
        char jackfilepath[100];
        char xmlfilepath[100];

        // 相対パスと出力先のxmlファイルの生成
        strcpy(jackfilepath, filename);

        size_t tmp = strlen(filename) - strlen(".jack");
        strncpy(xmlfilepath, filename, tmp);
        xmlfilepath[tmp] = '\0';
        strcat(xmlfilepath, "T.xml");

        process(jackfilepath, xmlfilepath);
    }
    // いずれでもない場合
    else error("Error: invalid parameter, neither file nor directory.");

    return 0;
}
