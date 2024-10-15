/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mott <mott@student.42heilbronn.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:40:43 by mott              #+#    #+#             */
/*   Updated: 2024/10/15 18:11:46 by mott             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

Cookies and session management

1.	Initial HTTP Request (No Cookie):

		GET /home.html HTTP/1.1
		Host: example.com
		Connection: keep-alive
		User-Agent: Mozilla/5.0
		Accept: text/html

2.	 Server Checks for Cookie:
	- If no cookie is found (hasCookie == false):
		- The server creates a new session by generating a new UserID and stores it in a session map.
		- The server adds a Set-Cookie header to the HTTP response to set the new UserID for the client.

	a.	Create New UserID:
			std::string generateUserID() {
				static int counter = 1;
				return "UserID=" + std::to_string(counter++);
			}

	b.	Set Cookie in HTTP Header:
			std::string setCookieHeader(const std::string& userID) {
				return "Set-Cookie: " + userID + "; Path=/; HttpOnly";
			}

3.	HTTP Response with Set-Cookie: The server sends a new cookie (UserID) to the client in the response.

		HTTP/1.1 200 OK
		Content-Type: text/html
		Set-Cookie: UserID=123; Path=/; HttpOnly
		Content-Length: 1234

		<html>Welcome, new user!</html>

4.	Subsequent HTTP Request (with Cookie):
	After the client receives the cookie, it will automatically include the cookie in future requests to the server.

		GET /home2.html HTTP/1.1
		Host: example.com
		Cookie: UserID=123;
		Connection: keep-alive
		User-Agent: Mozilla/5.0
		Accept: text/html

5.	Server Identifies Returning User (has Cookie):
	The server checks if the HTTP request includes a valid cookie.
	If the cookie is present, it extracts the UserID to recognize the client.

6.	HTTP Response Recognizing the Client:
	The server sends a personalized response, indicating that the user has been recognized.

		HTTP/1.1 200 OK
		Content-Type: text/html
		Set-Cookie: UserID=123; Path=/; HttpOnly
		Content-Length: 1234

		<html>Welcome back, User 123!</html>

#include <unordered_map>
#include <string>

// Define a struct to store session data
struct SessionData {
	std::string userID;
};

std::unordered_map<std::string, SessionData> sessionMap;

// Check if UserID exists in the map
bool hasCookie = false;
std::string userID = extractUserIDFromCookie(requestHeader);

if (sessionMap.find(userID) != sessionMap.end()) {
	// UserID found, client is recognized
	hasCookie = true;
	SessionData session = sessionMap[userID];
}
else {
	// No UserID found, create a new session
	std::string newUserID = generateUserID();
	SessionData newSession = {newUserID};
	sessionMap[newUserID] = newSession;
	responseHeader += setCookieHeader(newUserID);
}
