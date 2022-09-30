// Template generated code from trgen 0.17.0

#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <atomic>
#include "ANTLRInputStream.h"
#include "ErrorListener.h"
#include "abbLexer.h"
#include "abbParser.h"
#include "abbParserBaseListener.h"
#include "abbParserBaseVisitor.h"


#include "CaseChangingCharStream.h"

std::string formatDuration(uint64_t duration) {
    std::stringstream oss;
    // duration is in microseconds units.
    long tseconds = duration / 1000000;
    long minutes = tseconds / 60;
    long seconds = tseconds % 60;
    long microseconds = duration % 1000000;
    oss << std::setfill('0')
        << minutes
        << ":"
        << std::setw(2)
        << seconds
        << "."
        << std::setw(6)
        << microseconds;
    return oss.str();
}

class MyListener : public abbParserBaseListener
{
	public:
		/* virtual */ void enterModule(abbParser::ModuleContext * ctx) override
		{
			std::cout << "enterModule" << std::endl;
		}
		/* virtual */ void enterModuleData(abbParser::ModuleDataContext * ctx) override
		{
			std::cout << "enterModuleData" << std::endl;
		}
};

int TryParse(std::vector<std::string>& args)
{
    bool show_tree = false;
    bool show_tokens = false;
    std::string * file_name = nullptr;
    std::string * input = nullptr;
    for (int i = 0; i < args.size(); ++i)
    {
        if (args[i] == "-tokens")
        {
            show_tokens = true;
            continue;
        }
        else if (args[i] == "-tree")
        {
            show_tree = true;
            continue;
        }
        else if (args[i] == "-input")
            input = & args[++i];
        else if (args[i] == "-file")
            file_name = & args[++i];
    }
    antlr4::CharStream* str = nullptr;
    if (input == nullptr && file_name == nullptr)
    {
        str = new antlr4::ANTLRInputStream(std::cin);
    } else if (input != nullptr)
    {
        str = new antlr4::ANTLRInputStream(*input);
    } else if (file_name != nullptr)
    {
        std::fstream fs(*file_name);
        str = new antlr4::ANTLRInputStream(fs);
    }
        antlr4::Lexer * lexer = new abbLexer(str);
    if (show_tokens)
    {
        for (int i = 0; ; ++i)
        {
            auto token = lexer->nextToken();
            std::cout << token->toString() << std::endl;
            if (token->getType() == antlr4::IntStream::EOF)
                break;
        }
        lexer->reset();
    }
    auto tokens = new antlr4::CommonTokenStream(lexer);
    auto * parser = new abbParser(tokens);
    auto listener_lexer = new ErrorListener();
    auto listener_parser = new ErrorListener();
    lexer->removeErrorListeners();
    parser->removeErrorListeners();
    lexer->addErrorListener(listener_lexer);
    parser->addErrorListener(listener_parser);
    auto before = std::chrono::steady_clock::now();
    auto tree = parser->module();
    auto after = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(after - before);
    if (listener_parser->had_error || listener_lexer->had_error)
    {
        std::cerr << "Parse failed." << std::endl;
    }
    else
    {
        std::cerr << "Parse succeeded." << std::endl;
    }
    if (show_tree)
    {
//        System.Console.Error.WriteLine(tree.ToStringTree(parser));
    }
    std::cerr << "Time: " << formatDuration(duration.count()) << std::endl;

    auto * my_listener = new MyListener();
    antlr4::tree::ParseTreeWalker * walker = new antlr4::tree::ParseTreeWalker();
    walker->walk(my_listener, tree);
    return 0;
}

int main(int argc, const char * argv[])
{
    std::vector <std::string> args;
    for (int i = 1; i < argc; ++i)
    {
        args.push_back(argv[i]);
    }   
    TryParse(args);
}

