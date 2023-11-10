D [0-9]
L [a-zA-Z_]
H [a-fA-F0-9]
E [Ee][+-]?{D}+

%{

#include "fbe.h"
#include "fbe-parser.hpp"
#include "generator_cpp.h"
#include "generator_csharp.h"
#include "generator_go.h"
#include "generator_java.h"
#include "generator_javascript.h"
#include "generator_kotlin.h"
#include "generator_python.h"
#include "generator_ruby.h"
#include "generator_swift.h"

#include <optparse/optparse.hpp>
#include <string/string.hpp>
#include <iostream>

void yycount();
int yychar(char c);
int yytoken(int t);
int yystring(int s);
int yyerror(const char* msg);
int yyerror(const std::string& msg);

%}

%option noinput
%option nounput
%option noyywrap
%option yylineno

%%

"//"[^\n]*              { /* Single line comment */ }
"/*"[^*]*|[*]*"*/"      { /* Multilines comment */ }

"domain"                { yycount(); return yytoken(PDOMAIN); }
"package"               { yycount(); return yytoken(PACKAGE); }
"offset"                { yycount(); return yytoken(OFFSET); }
"import"                { yycount(); return yytoken(IMPORT); }
"version"               { yycount(); return yytoken(VERSION); }
"enum"                  { yycount(); return yytoken(ENUM); }
"flags"                 { yycount(); return yytoken(FLAGS); }
"variant"               { yycount(); return yytoken(VARIANT); }
"struct"                { yycount(); return yytoken(STRUCT); }
"message"               { yycount(); return yytoken(MESSAGE); }
"base"                  { yycount(); return yytoken(BASE); }
"[id]"                  { yycount(); return yytoken(ID); }
"[key]"                 { yycount(); return yytoken(KEY); }
"[hidden]"              { yycount(); return yytoken(HIDDEN); }
"[deprecated]"          { yycount(); return yytoken(DEPRECATED); }
"request"               { yycount(); return yytoken(REQ); }
"response"              { yycount(); return yytoken(RES); }
"reject"                { yycount(); return yytoken(REJ); }

"bool"                  { yycount(); return yystring(BOOL); }
"byte"                  { yycount(); return yystring(BYTE); }
"bytes"                 { yycount(); return yystring(BYTES); }
"char"                  { yycount(); return yystring(CHAR); }
"wchar"                 { yycount(); return yystring(WCHAR); }
"int8"                  { yycount(); return yystring(INT8); }
"uint8"                 { yycount(); return yystring(UINT8); }
"int16"                 { yycount(); return yystring(INT16); }
"uint16"                { yycount(); return yystring(UINT16); }
"int32"                 { yycount(); return yystring(INT32); }
"uint32"                { yycount(); return yystring(UINT32); }
"int64"                 { yycount(); return yystring(INT64); }
"uint64"                { yycount(); return yystring(UINT64); }
"float"                 { yycount(); return yystring(FLOAT); }
"double"                { yycount(); return yystring(DOUBLE); }
"decimal"               { yycount(); return yystring(DECIMAL); }
"string"                { yycount(); return yystring(STRING); }
"timestamp"             { yycount(); return yystring(TIMESTAMP); }
"uuid"                  { yycount(); return yystring(UUID); }

"true"                  { yycount(); return yystring(CONST_TRUE); }
"false"                 { yycount(); return yystring(CONST_FALSE); }
"null"                  { yycount(); return yystring(CONST_NULL); }
"epoch"                 { yycount(); return yystring(CONST_EPOCH); }
"utc"                   { yycount(); return yystring(CONST_UTC); }
"uuid0"                 { yycount(); return yystring(CONST_UUID0); }
"uuid1"                 { yycount(); return yystring(CONST_UUID1); }
"uuid4"                 { yycount(); return yystring(CONST_UUID4); }

'(\\.|[^\\'])+'         { yycount(); return yystring(CONST_CHAR); }
[-]?0[xX]{H}+           { yycount(); return yystring(CONST_INT); }
[-]?0{D}+               { yycount(); return yystring(CONST_INT); }
[-]?{D}+                { yycount(); return yystring(CONST_INT); }
[-]?{D}+{E}             { yycount(); return yystring(CONST_FLOAT); }
[-]?{D}*"."{D}+({E})?   { yycount(); return yystring(CONST_FLOAT); }
[-]?{D}+"."{D}*({E})?   { yycount(); return yystring(CONST_FLOAT); }
\"(\\.|[^\\"\n])*\"     { yycount(); return yystring(CONST_STRING); }

{L}({L}|{D})*           { yycount(); return yystring(IDENTIFIER); }

","                     { yycount(); return yychar(','); }
"."                     { yycount(); return yychar('.'); }
":"                     { yycount(); return yychar(':'); }
";"                     { yycount(); return yychar(';'); }
"+"                     { yycount(); return yychar('+'); }
"*"                     { yycount(); return yychar('*'); }
"="                     { yycount(); return yychar('='); }
"|"                     { yycount(); return yychar('|'); }
"!"                     { yycount(); return yychar('!'); }
"?"                     { yycount(); return yychar('?'); }
"~"                     { yycount(); return yychar('~'); }
"("                     { yycount(); return yychar('('); }
")"                     { yycount(); return yychar(')'); }
"["                     { yycount(); return yychar('['); }
"]"                     { yycount(); return yychar(']'); }
"<"                     { yycount(); return yychar('<'); }
">"                     { yycount(); return yychar('>'); }
"{"                     { yycount(); return yychar('{'); }
"}"                     { yycount(); return yychar('}'); }

[ \t\n\r\b\v\f]         { yycount(); }

.                       { yycount(); yyerror("Unknown token!"); }

%%

int yycolumn = 0;
std::string yyfile;
std::string yyline;

void yycount()
{
    for (int i = 0; yytext[i] != '\0'; ++i)
    {
        yyline += yytext[i];
        if (yytext[i] == '\n')
        {
            yycolumn = 0;
            yyline.clear();
        }
        else if (yytext[i] == '\t')
            yycolumn += 8 - (yycolumn % 8);
        else
            ++yycolumn;
    }
}

int yychar(char c)
{
    return c;
}

int yytoken(int t)
{
    yylval.token = t;
    return t;
}

int yystring(int s)
{
    yylval.string = new std::string(yytext, yyleng);
    return s;
}

int yyerror(const char* msg)
{
    if (yyfile.empty())
        yyfile = "<stdin>";
    fprintf(stderr, "\n%s:%d: error: %s\n", yyfile.c_str(), yylineno, msg);
    fprintf(stderr, "%s\n", yyline.c_str());
    fprintf(stderr, "%*s--- %s\n", yycolumn, "^", msg);
    yyterminate();
    return -1;
}

int yyerror(const std::string& msg)
{
    return yyerror(msg.c_str());
}

extern int yyparse();

int main(int argc, const char** argv)
{
    // auto parser = stdb::optparse::OptionParser().version(FBE::version);
    stdb::optparse::OptionParser parser;
    parser.version(FBE::version);
    using stdb::optparse::Action;
    using stdb::optparse::Type;
    using stdb::optparse::OptionParser;
    using stdb::memory::string;

    parser.add_option("-i", "--input").dest("input").type(Type::String).action(Action::Store).help("Input path");
    parser.add_option("-o", "--output").dest("output").type(Type::String).action(Action::Store).help("Output path");
    parser.add_option("-q", "--quiet").dest("quiet").type(Type::Bool).action(Action::StoreTrue).help("Launch in quiet mode. No progress will be shown!");
    parser.add_option("-n", "--indent").dest("indent").action(Action::Store).type(Type::Int).default_value(0).help("Format indent. Default: 0");
    parser.add_option("-t", "--tabs").dest("tabs").type(Type::Bool).action(Action::StoreTrue).default_value(0).help("Format with tabs. Default: off");
    parser.add_option("--cpp").dest("cpp").type(Type::Bool).action(Action::StoreTrue).default_value(0).help("Generate C++ code");
    parser.add_option("--cpp-logging").type(Type::Bool).dest("cpp-logging").action(Action::StoreTrue).default_value(0).help("Generate C++ logging code");
    parser.add_option("--csharp").type(Type::Bool).dest("csharp").action(Action::StoreTrue).default_value(0).help("Generate C# code");
    parser.add_option("--go").type(Type::Bool).dest("go").action(Action::StoreTrue).default_value(0).help("Generate Go code");
    parser.add_option("--go-module-path").type(Type::String).dest("go-module-path").action(Action::Store).help("Set go module path. Default: ..");
    parser.add_option("--java7").type(Type::Bool).dest("java7").action(Action::StoreTrue).default_value(0).help("Generate Java code (old Java 7 for Android)");
    parser.add_option("--java").type(Type::Bool).dest("java").action(Action::StoreTrue).default_value(0).help("Generate Java code");
    parser.add_option("--javascript").type(Type::Bool).dest("javascript").action(Action::StoreTrue).default_value(0).help("Generate JavaScript code");
    parser.add_option("--kotlin7").type(Type::Bool).dest("kotlin7").action(Action::StoreTrue).default_value(0).help("Generate Kotlin code (old Java 7 for Android)");
    parser.add_option("--kotlin").type(Type::Bool).dest("kotlin").action(Action::StoreTrue).default_value(0).help("Generate Kotlin code");
    parser.add_option("--python").type(Type::Bool).dest("python").action(Action::StoreTrue).default_value(0).help("Generate Python code");
    parser.add_option("--ruby").type(Type::Bool).dest("ruby").action(Action::StoreTrue).default_value(0).help("Generate Ruby code");
    parser.add_option("--swift").type(Type::Bool).dest("swift").action(Action::StoreTrue).default_value(0).help("Generate Swift code");
    parser.add_option("--final").type(Type::Bool).dest("final").action(Action::StoreTrue).default_value(0).help("Generate Final serialization code");
    parser.add_option("--json").type(Type::Bool).dest("json").action(Action::StoreTrue).default_value(0).help("Generate JSON serialization code");
    parser.add_option("--proto").type(Type::Bool).dest("proto").action(Action::StoreTrue).default_value(0).help("Generate Sender/Receiver protocol code");
    parser.add_option("--ptr").type(Type::Bool).dest("ptr").action(Action::StoreTrue).default_value(0).help("Generate pointer-based serialization code, conflicts with --final");
    parser.add_option("--import-ptr").type(Type::Bool).dest("import-ptr").action(Action::StoreTrue).default_value(0).help("import ptr-based FBE package");

    auto options = parser.parse_args(argc, argv);

    // Print help
    if (options.get<bool>("help").value_or(false))
    {
        parser.print_help();
        return 0;
    }

    // Print usage
    if (options.get<bool>("usage").value_or(false))
    {
        parser.print_usage();
        return 0;
    }

    // Client parameters
    string directory;
    auto input_opt = options.get<string>("input");
    auto output_opt = options.get<string>("output");
    auto indent_opt = options.get<int>("indent");
    bool tabs = options.get<bool>("tabs").value_or(false);
    bool quiet = options.get<bool>("quiet").value_or(false);
    bool cpp = options.get<bool>("cpp").value_or(false);
    bool cpp_logging = options.get<bool>("cpp-logging").value_or(false);
    bool csharp = options.get<bool>("csharp").value_or(false);
    bool go = options.get<bool>("go").value_or(false);
    bool java = options.get<bool>("java").value_or(false);
    bool java7 = options.get<bool>("java7").value_or(false);
    bool javascript = options.get<bool>("javascript").value_or(false);
    bool kotlin = options.get<bool>("kotlin").value_or(false);
    bool kotlin7 = options.get<bool>("kotlin7").value_or(false);
    bool python = options.get<bool>("python").value_or(false);
    bool ruby = options.get<bool>("ruby").value_or(false);
    bool swift = options.get<bool>("swift").value_or(false);
    bool final = options.get<bool>("final").value_or(false);
    bool json = options.get<bool>("json").value_or(false);
    bool proto = options.get<bool>("proto").value_or(false);
    bool ptr = options.get<bool>("ptr").value_or(false);
    bool import_ptr = options.get<bool>("import-ptr").value_or(false);
    auto go_module_path_opt = options.get<string>("go-module-path");
    char space = tabs ? '\t' : ' ';

    std::string input{input_opt.value_or("")};
    std::string output{output_opt.value_or("")};
    std::string go_module_path{go_module_path_opt.value_or("..")};
    int indent = indent_opt.value_or(0);

    // Check for the input path
    if (input.empty())
    {
        std::cerr << "Input path is not provided!" << std::endl;
        parser.print_help();
        return -1;
    }

    // Check for the output path
    if (output.empty())
    {
        std::cerr << "Output path is not provided!" << std::endl;
        parser.print_help();
        return -1;
    }

    // Open the input file
    if (!quiet) std::cout << "Open input file - " + input + "...";

    //CppCommon::File file(input);
    fs::path file(input);

    // Check if the input file exist
    //if (!file.IsFileExists())
    if (not fs::exists(file))
        yyerror("Input file not exist - " + input);

    // Decompose the parent directory
    directory = file.parent_path().string();

    // Open the input file
    FILE* in = fopen(input.c_str(), "r");

    // Setup the lexical parser input
    yyfile = input;
    yyset_in(in);

    if (!quiet) std::cout << "Done!" << std::endl;

    // Parse the input
    if (!quiet) std::cout << "Parsing input...";
    yyparse();
    if (!quiet) std::cout << "Done!" << std::endl;

    // Close the input file
    if (!input.empty())
    {
        if (!quiet) std::cout << "Close input file...";
        fclose(yyget_in());
        if (!quiet) std::cout << "Done!" << std::endl;
    }

    // Generate the output
    if (cpp)
    {
        if (!quiet) std::cout << "Generating arena C++ code...";
        FBE::GeneratorCpp(input, output, indent ? indent : 4, space)
            .Final(final)
            .JSON(json)
            .Proto(proto)
            .Logging(cpp_logging)
            .Ptr(ptr)
            .Arena(true)
            .ImportPtr(import_ptr)
            .Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;

        if (!quiet) std::cout << "Generating non-arena C++ code...";
        FBE::GeneratorCpp(input, output, indent ? indent : 4, space)
            .Final(final)
            .JSON(json)
            .Proto(proto)
            .Logging(cpp_logging)
            .Ptr(ptr)
            .Arena(false)
            .ImportPtr(import_ptr)
            .Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;
    }
    if (csharp)
    {
        if (!quiet) std::cout << "Generating C# code...";
        FBE::GeneratorCSharp(input, output, indent ? indent : 4, space).Final(final).JSON(json).Proto(proto).Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;
    }
    if (go)
    {
        if (!quiet) std::cout << "Generating Go code...";
        FBE::GeneratorGo(input, output, indent ? indent : 4 , space).Final(final).JSON(json).Proto(proto).ModulePath(go_module_path).Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;
    }
    if (java || java7)
    {
        if (!quiet) std::cout << "Generating Java code...";
        FBE::GeneratorJava(input, output, indent ? indent : 4, space).Final(final).JSON(json).Proto(proto).Version(java7 ? 7 : 8).Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;
    }
    if (javascript)
    {
        if (!quiet) std::cout << "Generating JavaScript code...";
        FBE::GeneratorJavaScript(input, output, indent ? indent : 2, space).Final(final).JSON(json).Proto(proto).Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;
    }
    if (kotlin || kotlin7)
    {
        if (!quiet) std::cout << "Generating Kotlin code...";
        FBE::GeneratorKotlin(input, output, indent ? indent : 4, space).Final(final).JSON(json).Proto(proto).Version(kotlin7 ? 7 : 8).Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;
    }
    if (python)
    {
        if (!quiet) std::cout << "Generating Python code...";
        FBE::GeneratorPython(input, output, indent ? indent : 4, space).Final(final).JSON(json).Proto(proto).Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;
    }
    if (ruby)
    {
        if (!quiet) std::cout << "Generating Ruby code...";
        FBE::GeneratorRuby(input, output, indent ? indent : 2, space).Final(final).JSON(json).Proto(proto).Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;
    }
    if (swift)
    {
        if (!quiet) std::cout << "Generating Swift code...";
        FBE::GeneratorSwift(input, output, indent ? indent : 4, space).Final(final).JSON(json).Proto(proto).Generate(FBE::Package::root);
        if (!quiet) std::cout << "Done!" << std::endl;
    }

    // Destroy the lexical parser
    yylex_destroy();

    return 0;
}
