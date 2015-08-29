#pragma once

#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <cstdint>

class Parser
{
public:
	void setStream(std::ifstream&& stream) { m_in = std::move(stream); }

	std::ifstream& stream() { return m_in; }
	std::streamoff tellg() { return m_in.tellg(); }
	void seekg(std::streampos pos) { m_in.seekg(pos); }
	void seekg(std::streamoff off, std::ios_base::seekdir dir) { m_in.seekg(off, dir); }
	void jump(std::streamoff off) { m_in.seekg(off, std::ios_base::cur); }
	bool eof() { return m_in.eof(); }

	template <class T, int N>
	Parser& operator>>(std::array<T, N>& a)
	{
		m_in.read(reinterpret_cast<char*>(a.data()), N * sizeof(T));
		return *this;
	}

	template <class T>
	Parser& operator>>(std::vector<T>& v)
	{
		if(!v.empty())
			m_in.read(reinterpret_cast<char*>(v.data()), v.size() * sizeof(T));
		return *this;
	}

	template <class T>
	Parser& operator>>(T& t)
	{
		m_in.read(reinterpret_cast<char*>(&t), sizeof(T));
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
	std::ifstream m_in;

	template <class T>
	void read(T& t)
	{
		m_in.read(reinterpret_cast<char*>(&t), sizeof(T));
	}
};
