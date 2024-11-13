/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:00:00 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/14 12:00:00 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>

namespace Constants {

// Constantes HTTP
const std::string HTTP_VERSION = "HTTP/1.1";
const std::string CONNECTION_HEADER = "Connection";
const std::string KEEP_ALIVE = "keep-alive";
const std::string CLOSE = "close";
const std::string REQUEST_TERMINATOR = "\r\n\r\n";

// Séparateurs pour les en-têtes
const std::string HEADER_SEPARATOR = ": ";
const std::string LINE_TERMINATOR = "\r\n";

// Répertoire racine par défaut
const std::string DEFAULT_ROOT_DIRECTORY = "www";

// Chemins des pages d'erreur
const std::string ERROR_PAGES_DIRECTORY = "www/errorsPages/";

// Fichier index par défaut
const std::string DEFAULT_INDEX_FILE = "index/index.html";

// Types de contenu par extension de fichier
inline const std::map<std::string, std::string>& getContentTypes()
{
    static std::map<std::string, std::string> contentTypes;
    if (contentTypes.empty()) {
        contentTypes[".html"] = "text/html";
        contentTypes[".htm"] = "text/html";
        contentTypes[".css"] = "text/css";
        contentTypes[".js"] = "application/javascript";
        contentTypes[".png"] = "image/png";
        contentTypes[".jpg"] = "image/jpeg";
        contentTypes[".jpeg"] = "image/jpeg";
        contentTypes[".gif"] = "image/gif";
        contentTypes[".txt"] = "text/plain";
        contentTypes[".pdf"] = "application/pdf";
        // Ajouter d'autres types si nécessaire
    }
    return contentTypes;
}

// Messages de log
const std::string LOG_CLIENT_COPIED = "Client copié avec le descripteur : ";
const std::string LOG_RESPONSE_CONFIGURED = "Réponse configurée : ";
const std::string LOG_SEND_BYTES = "Envoyé ";
const std::string LOG_SEND_BYTES_TO_CLIENT = " octets au client ";
const std::string LOG_ERROR_IN_FUNCTION = "Erreur dans ";
const std::string LOG_CLOSING_CONNECTION_FOR_CLIENT = "Fermeture de la connexion pour le client ";
const std::string LOG_MODIFYING_EPOLL_EVENTS_FOR_CLIENT = "Modification des événements epoll pour le client ";
const std::string LOG_DELETED_CLIENT_FROM_LIST = "Client supprimé de la liste des clients.";
const std::string LOG_EPOLL_CREATED_SUCCESS = "epoll créé avec succès, descripteur : ";
const std::string LOG_STARTING_TO_LISTEN = "----------STARTING TO LISTEN----------";
const std::string LOG_EVENTS_RECEIVED = "Nombre d'événements reçus : ";

// Limites
const int SOCKET_RECV_BUFFER_SIZE = 65536; // 64KB
const int SOCKET_SEND_BUFFER_SIZE = 65536; // 64KB

// Codes de statut HTTP et leurs phrases correspondantes
inline const std::map<int, std::string>& getStatusPhrases()
{
    static std::map<int, std::string> statusPhrases;
    if (statusPhrases.empty()) {
        statusPhrases[200] = "OK";
        statusPhrases[400] = "Bad Request";
        statusPhrases[404] = "Not Found";
        statusPhrases[405] = "Method Not Allowed";
        statusPhrases[411] = "Length Required";
        statusPhrases[413] = "Request Entity Too Large";
        statusPhrases[500] = "Internal Server Error";
        statusPhrases[501] = "Not Implemented";
        statusPhrases[502] = "Bad Gateway";
        statusPhrases[503] = "Service Unavailable";
        statusPhrases[504] = "Gateway Timeout";
        statusPhrases[505] = "HTTP Version Not Supported";
    }
    return statusPhrases;
}

// Définition des constantes manquantes
const int DEFAULT_STATUS_CODE = 200;
const std::string DEFAULT_REASON_PHRASE = "OK";

// Codes de statut HTTP
const int OK = 200;
const int BAD_REQUEST = 400;
const int PAGE_NOT_FOUND = 404;
const int METHOD_NOT_ALLOWED = 405;
const int LENGTH_REQUIRED = 411;
const int REQUEST_ENTITY_TOO_LARGE = 413;
const int INTERNAL_SERVER_ERROR = 500;
const int NOT_IMPLEMENTED = 501;
const int BAD_GATEWAY = 502;
const int SERVICE_UNAVAILABLE = 503;
const int GATEWAY_TIMEOUT = 504;
const int HTTP_VERSION_NOT_SUPPORTED = 505;

// Ajoutez SUPPORTED_METHODS_COUNT ici
const size_t SUPPORTED_METHODS_COUNT = 3;

// Définition des Méthodes HTTP supportées
extern const char* SUPPORTED_METHODS[];

} // namespace Constants
