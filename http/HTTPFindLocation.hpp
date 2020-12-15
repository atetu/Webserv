/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPFindLocation.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alicetetu <alicetetu@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 15:24:48 by alicetetu         #+#    #+#             */
/*   Updated: 2020/12/15 16:59:59 by alicetetu        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPFINDLOCATION_HPP_
# define HTTPFINDLOCATION_HPP_

#include <config/block/CGIBlock.hpp>
#include <config/block/ServerBlock.hpp>

class HTTPFindLocation
{
	public:
		enum State
		{
			S_NOT_STARTED,
			S_START,
			S_END,
		//	S_SPACES_BEFORE_VALUE,
			// S_SPACES_AFTER_VALUE,
			// S_SEMI_COLON,
			// S_WEIGHT_START1,
			// S_WEIGHT_START2,
			// S_SPACES_BEFORE_WEIGHT,
			// S_WEIGHT,
			// S_SPACES_AFTER_WEIGHT,
			// S_END,
			// S_END2,
		};
	
	private:
		State m_state;
		std::string m_clientPath;
		std::vector<LocationBlock*> m_serverLocations;
		LocationBlock* m_locationBlock;
		
	public:
		HTTPFindLocation();
		HTTPFindLocation(std::string clientPath, std::vector<LocationBlock*> serverBlock);
		HTTPFindLocation(const HTTPFindLocation &other);
		
		virtual
		~HTTPFindLocation();

		HTTPFindLocation&
		operator =(const HTTPFindLocation &other);

		LocationBlock*
		location(void);

		void
		parse(void);

		void
		match(std::string path);
};

#endif /* HTTPFindLocation_HPP_ */