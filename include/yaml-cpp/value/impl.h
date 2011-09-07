#ifndef VALUE_IMPL_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define VALUE_IMPL_H_62B23520_7C8E_11DE_8A39_0800200C9A66

#if defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ >= 4)) // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif


#include "yaml-cpp/value/value.h"
#include "yaml-cpp/value/detail/memory.h"
#include "yaml-cpp/value/detail/node.h"
#include <string>

namespace YAML
{
	inline Value::Value(): m_pMemory(new detail::memory_holder), m_pNode(m_pMemory->create_node())
	{
		m_pNode->set_null();
	}
	
	inline Value::Value(ValueType::value type): m_pMemory(new detail::memory_holder), m_pNode(m_pMemory->create_node())
	{
		m_pNode->set_type(type);
	}
	
	template<typename T>
	inline Value::Value(const T& rhs): m_pMemory(new detail::memory_holder), m_pNode(m_pMemory->create_node())
	{
		Assign(rhs);
	}
	
	inline Value::Value(const Value& rhs): m_pMemory(rhs.m_pMemory), m_pNode(rhs.m_pNode)
	{
	}
	
	inline Value::Value(detail::shared_node pNode, detail::shared_memory_holder pMemory): m_pMemory(pMemory), m_pNode(pNode)
	{
	}

	inline Value::~Value()
	{
	}

	inline ValueType::value Value::Type() const
	{
		return m_pNode->type();
	}
	
	// access
	template<typename T>
	inline const T Value::as() const
	{
		T t;
		if(convert<T>(*this, t))
			return t;
		throw std::runtime_error("Unable to convert to type");
	}
	
	// assignment
	template<typename T>
	inline Value& Value::operator=(const T& rhs)
	{
		Assign(rhs);
		return *this;
	}
	
	template<typename T>
	inline void Value::Assign(const T& rhs)
	{
		AssignData(convert<T>(rhs));
	}

	template<>
	inline void Value::Assign(const std::string& rhs)
	{
		m_pNode->set_scalar(rhs);
	}

	inline void Value::Assign(const char *rhs)
	{
		m_pNode->set_scalar(rhs);
	}

	inline void Value::Assign(char *rhs)
	{
		m_pNode->set_scalar(rhs);
	}
	
	inline Value& Value::operator=(const Value& rhs)
	{
		if(is(*this, rhs))
			return *this;
		AssignNode(rhs);
		return *this;
	}

	void Value::AssignData(const Value& rhs)
	{
		m_pNode->set_data(*rhs.m_pNode);
		m_pMemory->merge(*rhs.m_pMemory);
	}

	void Value::AssignNode(const Value& rhs)
	{
		m_pNode = rhs.m_pNode;
		m_pMemory->merge(*rhs.m_pMemory);
	}

	// size/iterator
	inline std::size_t Value::size() const
	{
		return 0;
	}

	inline const_iterator Value::begin() const
	{
		return const_iterator();
	}
	
	inline iterator Value::begin()
	{
		return iterator();
	}

	inline const_iterator Value::end() const
	{
		return const_iterator();
	}

	inline iterator Value::end()
	{
		return iterator();
	}
	
	// indexing
	template<typename Key>
	inline const Value Value::operator[](const Key& key) const
	{
		detail::shared_node pValue = static_cast<const detail::node&>(*m_pNode).get(key, m_pMemory);
		return Value(pValue, m_pMemory);
	}
	
	template<typename Key>
	inline Value Value::operator[](const Key& key)
	{
		detail::shared_node pValue = m_pNode->get(key, m_pMemory);
		return Value(pValue, m_pMemory);
	}
	
	template<typename Key>
	inline bool Value::remove(const Key& key)
	{
		return m_pNode->remove(key, m_pMemory);
	}
	
	inline const Value Value::operator[](const Value& key) const
	{
		detail::shared_node pValue = static_cast<const detail::node&>(*m_pNode).get(key.m_pNode);
		return Value(pValue, m_pMemory);
	}
	
	inline Value Value::operator[](const Value& key)
	{
		detail::shared_node pValue = m_pNode->get(key.m_pNode);
		return Value(pValue, m_pMemory);
	}
	
	inline bool Value::remove(const Value& key)
	{
		return m_pNode->remove(key.m_pNode);
	}
	
	inline const Value Value::operator[](const char *key) const
	{
		return operator[](std::string(key));
	}
	
	inline Value Value::operator[](const char *key)
	{
		return operator[](std::string(key));
	}
	
	inline bool Value::remove(const char *key)
	{
		return remove(std::string(key));
	}
	
	inline const Value Value::operator[](char *key) const
	{
		return operator[](static_cast<const char *>(key));
	}
	
	inline Value Value::operator[](char *key)
	{
		return operator[](static_cast<const char *>(key));
	}
	
	inline bool Value::remove(char *key)
	{
		return remove(static_cast<const char *>(key));
	}

	// free functions
	inline int compare(const Value& lhs, const Value& rhs)
	{
		return 0;
	}
	
	inline bool operator<(const Value& lhs, const Value& rhs)
	{
		return false;
	}
	
	inline bool is(const Value& lhs, const Value& rhs)
	{
		return false;
	}
	
	template<typename T>
	inline bool equals(detail::shared_node pNode, const T& rhs, detail::shared_memory_holder pMemory)
	{
		T lhs;
		if(convert(Value(pNode, pMemory), lhs))
			return lhs == rhs;
		return false;
	}
}

#endif // VALUE_IMPL_H_62B23520_7C8E_11DE_8A39_0800200C9A66
