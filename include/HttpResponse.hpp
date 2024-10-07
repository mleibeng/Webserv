/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:56:37 by fwahl             #+#    #+#             */
/*   Updated: 2024/10/07 15:56:38 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>

class HttpResponse
{
	public:
		HttpResponse();
		HttpResponse(const HttpResponse &other);
		HttpResponse& operator=(const HttpResponse &other);
		~HttpResponse();

	private:

};

#endif // HTTPRESPONSE_H
