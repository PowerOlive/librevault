/* Copyright (C) 2015 Alexander Shishenko <GamePad64@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include "PortMappingService.h"
#include "util/Loggable.h"
#include <natpmp.h>

namespace librevault {

class Client;

class P2PProvider;

class NATPMPService : public PortMappingService, public Loggable {
public:
	NATPMPService(Client& client);
	virtual ~NATPMPService();

	void reload_config();

	void add_port_mapping(const std::string& id, MappingDescriptor descriptor, std::string description);
	void remove_port_mapping(const std::string& id);

	Client& client_;
	std::mutex natpmp_lock_;

protected:
	natpmp_t natpmp;

	class PortMapping {
	public:
		PortMapping(NATPMPService& parent, MappingDescriptor descriptor);
		~PortMapping();

		void reload_config();

	private:
		NATPMPService& parent_;
		MappingDescriptor descriptor_;

		boost::asio::system_timer maintain_timer_;

		bool active = false;

		void send_request(bool unmap, const boost::system::error_code& error = boost::system::error_code());
	};
	friend class PortMapping;

	std::map<std::string, std::unique_ptr<PortMapping>> mappings_;
};

} /* namespace librevault */
