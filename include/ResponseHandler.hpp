/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:22:24 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/13 17:12:47 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HeaderBuilder.hpp"
#include "HttpRequest.hpp" // Assurez-vous que ce fichier est correctement défini
#include <fstream> // Pour la lecture des fichiers
#include <sstream>
#include <string>

class ResponseHandler {
public:
    ResponseHandler(const RequestParsed& requestParsed);
    ~ResponseHandler();

    void handleResponse(); // Méthode principale pour gérer la réponse
    std::string getResponse() const; // Méthode pour obtenir la réponse complète

private:
    const RequestParsed& requestParsed; // Référence constante de la requête analysée
    HeaderBuilder headerBuilder;
    std::string body; // Contenu du corps de la réponse
    std::string fullResponse; // Contient la réponse complète

    void buildResponse(); // Construire la réponse principale
    std::string determineFilePath() const; // Déterminer le chemin du fichier pour la requête
    void prepareSuccessResponse(const std::string& filePath); // Préparer la réponse pour une requête réussie
    void prepareErrorResponse(int statusCode); // Préparer la réponse pour une erreur
    void assembleFullResponse(int statusCode); // Assembler la réponse finale

    std::string getReasonPhrase(int code) const; // Obtenir la phrase de raison pour le code de statut
    std::string getFilePathFromUri(const std::string& uri) const; // Convertir URI en chemin de fichier
    bool fileExists(const std::string& filePath) const; // Vérifier si le fichier existe
    std::string getContentType(const std::string& filePath) const; // Déterminer le type de contenu
    std::string readFile(const std::string& filePath) const; // Lire le contenu du fichier
};
