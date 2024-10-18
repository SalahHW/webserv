/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 04:06:19 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/11 04:06:19 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>

// Constructeur et destructeur
ConfigFile::ConfigFile(const std::string& configFilePath)
    : mainBlock(NULL)
    , isValid(true)
{
    readConfigFile(configFilePath);
}

ConfigFile::~ConfigFile()
{
    delete mainBlock;
}

// Accesseurs
const Block& ConfigFile::getMainBlock() const
{
    return *mainBlock;
}

bool ConfigFile::good() const
{
    return isValid;
}

// Méthode principale de lecture du fichier de configuration
void ConfigFile::readConfigFile(const std::string& fileName)
{
    std::ifstream file;
    utils::openFile(fileName, file);

    mainBlock = BlockFactory().createMainBlock();
    std::stack<Block*> blockStack;
    blockStack.push(mainBlock);

    parseFile(file, blockStack);

    file.close();

    // Vérification que tous les blocs ont été fermés correctement
    if (isValid && blockStack.size() > 1) {
        std::cerr << "Error: Some blocks were not closed properly." << std::endl;
        // Afficher les blocs non fermés
        while (blockStack.size() > 1) { // Exclure mainBlock
            Block* unclosedBlock = blockStack.top();
            std::cerr << "Unclosed block: \"" << unclosedBlock->getName() << "\"" << std::endl;
            blockStack.pop();
        }
        isValid = false;
    }
}

// Parse le fichier ligne par ligne
void ConfigFile::parseFile(std::ifstream& file, std::stack<Block*>& blockStack)
{
    std::string line;
    std::string accumulatedLine;

    while (std::getline(file, line)) {
        std::string cleanedLine = cleanLine(line);

        if (!cleanedLine.empty()) {
            accumulatedLine += cleanedLine + " ";

            // Vérifier si la ligne est complète
            if (lineEndsProperly(accumulatedLine)) {
                handleLine(accumulatedLine, file, blockStack);
                accumulatedLine.clear();
                if (!isValid) {
                    break; // Arrêter l'analyse en cas d'invalidité
                }
            }
            // Sinon, continuer à accumuler les lignes
        }
    }

    // Traiter la dernière ligne accumulée s'il y en a une
    if (!accumulatedLine.empty()) {
        handleLine(accumulatedLine, file, blockStack);
    }
}

// Vérifie si une ligne se termine correctement par ';', '{' ou '}'
bool ConfigFile::lineEndsProperly(const std::string& line) const
{
    std::string trimmedLine = utils::trimWhitespace(line);
    if (trimmedLine.empty()) {
        return false;
    }

    // Vérifier si la ligne se termine par ';', '{' ou '}'
    char lastChar = trimmedLine[trimmedLine.size() - 1];
    return (lastChar == ';' || lastChar == '{' || lastChar == '}');
}

// Gère une ligne nettoyée du fichier de configuration
void ConfigFile::handleLine(const std::string& accumulatedLine, std::ifstream& file, std::stack<Block*>& blockStack)
{
    std::string cleanedLine = utils::trimWhitespace(accumulatedLine);
    Block* currentBlock = blockStack.top();

    if (cleanedLine.empty()) {
        return;
    }
    if (cleanedLine == "{") {
        // Rien à faire
    } else if (cleanedLine == "}") {
        if (blockStack.size() > 1) {
            blockStack.pop();
        } else {
            std::cerr << "Error: Unmatched closing brace '}' in block \"" << currentBlock->getName() << "\"." << std::endl;
            isValid = false;
        }
    } else if (isDirective(cleanedLine)) {
        processDirective(cleanedLine, currentBlock);
    } else {
        bool onlyClosingBraces = true;
        for (size_t i = 0; i < cleanedLine.size(); ++i) {
            if (cleanedLine[i] != '}') {
                onlyClosingBraces = false;
                break;
            }
        }

        if (onlyClosingBraces) {
            for (size_t i = 0; i < cleanedLine.size(); ++i) {
                if (blockStack.size() > 1) {
                    blockStack.pop();
                } else {
                    std::cerr << "Error: Unmatched closing brace '}' in block \"" << currentBlock->getName() << "\"." << std::endl;
                    isValid = false;
                    return;
                }
            }
        } else {
            processBlockDeclaration(cleanedLine, file, blockStack, currentBlock);
        }
    }
}

// Gère une déclaration de bloc
void ConfigFile::processBlockDeclaration(const std::string& line, std::ifstream& file,
                                         std::stack<Block*>& blockStack, Block* currentBlock)
{
    std::string blockLine = line;
    std::string afterBlockName;
    bool braceFoundInLine = false;

    // Vérifier si la ligne contient une accolade ouvrante '{'
    std::size_t openBracePos = line.find('{');
    if (openBracePos != std::string::npos) {
        braceFoundInLine = true;
        blockLine = line.substr(0, openBracePos);
        afterBlockName = line.substr(openBracePos + 1);
    }

    // Extraire le nom du bloc
    std::string blockName = extractBlockName(blockLine);

    if (blockName.empty()) {
        std::cerr << "Error: Unnamed block encountered inside block \""
                  << currentBlock->getName() << "\"." << std::endl;
        isValid = false;
        return;
    }

    // Si l'accolade ouvrante n'est pas sur la même ligne, chercher dans les lignes suivantes
    if (!braceFoundInLine) {
        if (!findOpeningBrace(file, currentBlock)) {
            std::cerr << "Error: Expected '{' after block name \"" << blockName
                      << "\" inside block \"" << currentBlock->getName() << "\"." << std::endl;
            isValid = false;
            return;
        }
    } else {
        // Vérifier s'il y a du contenu après l'accolade ouvrante '{'
        std::string restLine = cleanLine(afterBlockName);
        if (!restLine.empty()) {
            std::cerr << "Error: Unexpected content after '{' in block \"" << blockName
                      << "\" inside block \"" << currentBlock->getName() << "\"." << std::endl;
            isValid = false;
            return;
        }
    }

    std::string cleanedLine = utils::removeBraces(line);
    Block* newBlock = BlockFactory().create(blockName, currentBlock, cleanedLine);

    if (!newBlock) {
        std::cerr << "Error: Block \"" << blockName << "\" in block \""
                  << currentBlock->getName() << "\" is not supported." << std::endl;
        isValid = false;
        return;
    }

    if (!newBlock->validateContext()) {
        delete newBlock;
        isValid = false;
        return;
    }

    // Ajouter le nouveau bloc en tant qu'enfant du bloc courant
    currentBlock->addSubBlock(newBlock);

    // Pousser le nouveau bloc sur la pile
    blockStack.push(newBlock);

    // Appeler parseBlock pour analyser le contenu du bloc
    parseBlock(file, blockStack);
}

// Analyse récursive du contenu d'un bloc
void ConfigFile::parseBlock(std::ifstream& file, std::stack<Block*>& blockStack)
{
    std::string line;
    std::string accumulatedLine;

    while (std::getline(file, line)) {
        std::string cleanedLine = cleanLine(line);

        if (!cleanedLine.empty()) {
            accumulatedLine += cleanedLine + " ";

            // Vérifier si la ligne est complète
            if (lineEndsProperly(accumulatedLine)) {
                handleLine(accumulatedLine, file, blockStack);
                accumulatedLine.clear();
                if (!isValid) {
                    return;
                }
            }
            // Sinon, continuer à accumuler les lignes
        }
    }

    // Traiter la dernière ligne accumulée s'il y en a une
    if (!accumulatedLine.empty()) {
        handleLine(accumulatedLine, file, blockStack);
    }

    // Si fin de fichier atteinte sans trouver '}'
    if (blockStack.size() > 1) {
        std::cerr << "Error: Block \"" << blockStack.top()->getName()
                  << "\" not closed properly (missing '}' before EOF)." << std::endl;
        isValid = false;
    }
}

// Traite une directive
void ConfigFile::processDirective(const std::string& cleanedLine, Block* currentBlock)
{
    if (cleanedLine.empty()) {
        std::cerr << "Unexpected empty directive in block \"" << currentBlock->getName()
                  << "\"." << std::endl;
        isValid = false;
        return;
    }

    if (cleanedLine[cleanedLine.size() - 1] != ';') {
        std::cerr << "Error: Missing ';' at the end of directive \"" << cleanedLine
                  << "\" in block \"" << currentBlock->getName() << "\"." << std::endl;
        isValid = false;
        return;
    }

    std::string directiveLine = cleanedLine.substr(0, cleanedLine.size() - 1); // Enlever le ';'
    std::string directiveName = extractDirectiveName(directiveLine);

    Directive* directive = directiveFactory.create(directiveName, currentBlock->getName(), directiveLine);
    if (!directive) {
        std::cerr << "Error: Directive \"" << directiveName << "\" in block \""
                  << currentBlock->getName() << "\" is not supported." << std::endl;
        isValid = false;
        return;
    }
    if (!directive->getIsValid()) {
        delete directive;
        isValid = false;
        return;
    }
    currentBlock->addDirective(directive);
}

// Vérifie si une ligne est une directive (se termine par ';' et pas de '{' avant ';')
bool ConfigFile::isDirective(const std::string& line) const
{
    std::string trimmedLine = utils::trimWhitespace(line);
    if (trimmedLine.empty()) {
        return false;
    }

    if (trimmedLine[trimmedLine.size() - 1] != ';') {
        return false;
    }

    std::size_t semiColonPos = trimmedLine.find_last_of(';');
    std::size_t bracePos = trimmedLine.find('{');

    if (bracePos != std::string::npos && bracePos < semiColonPos) {
        return false;
    }

    return true;
}

// Recherche une accolade ouvrante '{' dans le fichier
bool ConfigFile::findOpeningBrace(std::ifstream& file, Block* currentBlock)
{
    std::string line;
    while (std::getline(file, line)) {
        std::string cleanedLine = cleanLine(line);
        if (!cleanedLine.empty()) {
            if (cleanedLine == "{") {
                return true;
            } else {
                std::cerr << "Expected '{' but found: \"" << cleanedLine
                          << "\" inside block \"" << currentBlock->getName() << "\"." << std::endl;
                isValid = false;
                return false;
            }
        }
    }
    std::cerr << "End of file reached without finding '{' inside block \""
              << currentBlock->getName() << "\"." << std::endl;
    isValid = false;
    return false;
}

// Nettoie une ligne des commentaires et des espaces
std::string ConfigFile::cleanLine(const std::string& originalLine) const
{
    std::string cleanedLine = utils::removeComments(originalLine);
    cleanedLine = utils::trimWhitespace(cleanedLine);
    return cleanedLine;
}

// Extrait le nom du bloc d'une ligne
std::string ConfigFile::extractBlockName(const std::string& line) const
{
    std::stringstream ss(line);
    std::string blockName;
    ss >> blockName;
    return blockName;
}

// Extrait le nom de la directive d'une ligne
std::string ConfigFile::extractDirectiveName(const std::string& line) const
{
    std::stringstream ss(line);
    std::string directiveName;
    ss >> directiveName;
    return directiveName;
}