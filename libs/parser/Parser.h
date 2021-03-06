/******************************************************************************
*                            Skyrim Alchemy Helper                            *
*******************************************************************************
*                                                                             *
* Copyright (C) 2015 Christophe Guebert                                       *
*                                                                             *
* This program is free software; you can redistribute it and/or modify        *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation; either version 2 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc.,     *
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
*******************************************************************************
*                       Skyrim Alchemy Helper :: Parser                       *
*                                                                             *
*                  Contact: christophe.guebert+SAH@gmail.com                  *
******************************************************************************/
#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace parser
{

class Parser
{
public:
	void setStream(std::ifstream&& stream) { m_fstream = std::move(stream); m_in = &m_fstream; }
	void setStream(std::istringstream&& stream) { m_sstream = std::move(stream); m_in = &m_sstream; }

	std::istream& stream() { return *m_in; }
	std::istream*& streamPtr() { return m_in; }
	std::streamoff tellg() { return m_in->tellg(); }
	void seekg(std::streampos pos) { m_in->seekg(pos); }
	void seekg(std::streamoff off, std::ios_base::seekdir dir) { m_in->seekg(off, dir); }
	void jump(std::streamoff off);
	bool eof() { return m_in->eof(); }

	template <class T, int N>
	Parser& operator>>(std::array<T, N>& a)
	{
		m_in->read(reinterpret_cast<char*>(a.data()), N * sizeof(T));
		return *this;
	}

	template <class T>
	Parser& operator>>(std::vector<T>& v)
	{
		if (!v.empty())
			m_in->read(reinterpret_cast<char*>(v.data()), v.size() * sizeof(T));
		return *this;
	}

	template <class T>
	Parser& operator>>(T& t)
	{
		m_in->read(reinterpret_cast<char*>(&t), sizeof(T));
		return *this;
	}

	uint32_t readVSVal();

	std::string readBString(); // Not null terminated string prefixed with a byte length
	std::string readBZString(); // Null terminated string prefixed with a byte length
	std::string readWString(); // Not null terminated string prefixed with a short length
	std::string readWZString(); // Null terminated string prefixed with a short length
	std::string readZString(); // Null terminated string without size
	std::string readString(int size); // Not null terminated string of the given size

protected:
	std::istream* m_in = nullptr;
	std::ifstream m_fstream;
	std::istringstream m_sstream;

	template <class T>
	void read(T& t)
	{
		m_in->read(reinterpret_cast<char*>(&t), sizeof(T));
	}
};

} // namespace parser
