/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 02:32:48 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/18 03:30:17 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::RequestHandler()
{
	// std::cout << GREY << "Default constructor called" << RESET << std::endl;
}

RequestHandler::RequestHandler(const std::unordered_map<int, std::string>& _error_pages, const Config &config)
: _error_pages(_error_pages), config(config) {}

RequestHandler::~RequestHandler()
{
	// std::cout << GREY << "Destructor called" << RESET << std::endl;
}

std::string		RequestHandler::returnResponse(const HttpResponse &why_do_we_do_this_again)
{
	// warum konvertieren wir httpresponse wieder zu string um damit der client den string dann wieder verschicken kann??
	// naja hier ist die logik
	return(why_do_we_do_this_again.buildResponse());
}

std::string RequestHandler::serveErrorPage(int error_code)
{
	for (const auto& [code, string] : _error_pages)
	{
		if (code == error_code)
			return string;
	} // braucht auch noch die logik wenn wir keine error_pages haben
	  // und wenn wir unterschiedliche error_pages in unterschiedlichen routen haben,
	  // aber das muss eigentlich davor schon geschaut werden.
}

const ServerConf *RequestHandler::findServerConf(const HttpRequest &request)
{

}

const RouteConf *RequestHandler::findRouteConf(const ServerConf &server_conf, HttpRequest& request)
{

}

//sollten wir wirklich in einen string umwandeln vor dem zurueckgeben...
HttpResponse		RequestHandler::handleRequest(const HttpRequest& request, const Client& client)
{

	const ServerConf* server_conf = findServerConf(request);
	if (!server_conf)
	{
		// also wie gesagt wir koennen in serverErrorPage auch son Response objekt bauen,
		// dass dann die page als sich selbst verpackt
		// und dann muessen wir trotzdem irgendwie das wieder entpacken und in einen string umwandeln
			HttpResponse RueckgabeObjekt;
			RueckgabeObjekt.setStatus(StatusCode::NOT_IMPLEMENTED);
			RueckgabeObjekt.setBody(serveErrorPage(501));
			RueckgabeObjekt.setHeader("Content-Type", "html/txt");
			return RueckgabeObjekt;
		// so das muessen wir jetzt ueberall bei jedem serven von ner error page machen...
	}

	const RouteConf* route_conf = findRouteConf(*server_conf, request);
	if (!route_conf)
	{
		HttpResponse RueckgabeObjekt;
		RueckgabeObjekt.setStatus(StatusCode::NOT_IMPLEMENTED);
		RueckgabeObjekt.setBody(serveErrorPage(501));
		RueckgabeObjekt.setHeader("Content-Type", "html/txt");
		serveErrorPage(404); //nich gefunden
		return;
	}

	if (!isMethodAllowed(*route_conf, request.method))
	{
		HttpResponse RueckgabeObjekt;
		RueckgabeObjekt.setStatus(StatusCode::NOT_IMPLEMENTED);
		RueckgabeObjekt.setBody(serveErrorPage(501));
		RueckgabeObjekt.setHeader("Content-Type", "html/txt");
		serveErrorPage(405); //verboten
		return;
	}

	switch(request.getMethod())
	{
		case Method::GET:
			return (handleGetRequest(request, client));
		case Method::POST:
			return (handlePostRequest(request, client));
		case Method::DELETE:
			return (handleDeleteRequest(request, client));
		default:
		{
			HttpResponse response;
			response.setStatus(StatusCode::NOT_IMPLEMENTED);
			response.setBody(serveErrorPage(501)); // gibs nich
			response.setHeader("Content-Type", "html/txt");
			return response;
		}
	}
	// ok aber client sendet nen string, kein response class objekt
}

HttpResponse		RequestHandler::handleGetRequest(const HttpRequest& request, const Client& client)
{
	HttpResponse response;
	if (!std::filesystem::exists(request.getFilePath()))
	{
		response.setStatus(StatusCode::NOT_FOUND);
		response.setBody(serveErrorPage(404));
		response.setMimeType(".html");
	}

	std::ifstream		file(request.getFilePath());
	std::stringstream	fileContent;

	fileContent << file.rdbuf();
	response.setStatus(StatusCode::OK);
	response.setBody(fileContent.str());
	response.setMimeType(getFileExtension(request.getFilePath()));

	// warum nicht als string an den client damit der das verschicken kann? wofuer ist buildResponse() da?
}
HttpResponse		RequestHandler::handlePostRequest(const HttpRequest& request, const Client& client)
{
	// warum nicht als string an den client damit der das verschicken kann? wofuer ist buildResponse() da?
}

HttpResponse		RequestHandler::handleDeleteRequest(const HttpRequest& request, const Client& client)
{
	// warum nicht als string an den client damit der das verschicken kann? wofuer ist buildResponse() da?
}

std::string		getFileExtension(const std::string& filepath)
{
	return (std::filesystem::path(filepath).extension().string());
}
