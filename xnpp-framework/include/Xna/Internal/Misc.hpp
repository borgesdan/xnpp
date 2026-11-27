#pragma once

//MISC.HPP is a header with useful functions and classes.

#include <memory>
#include <utility>
#include <string>
#include <stdexcept>
#include <source_location>
#include <string>
#include <cstdint>
#include <algorithm>
#include <any>
#include <typeindex>
#include <cstddef>
#include <cstdlib>

namespace Xna::Misc {
	template <typename TENUM>
	static constexpr void SetFlag(int& current, TENUM flag) {
		current |= static_cast<int>(flag);
	}

	template <typename TENUM>
	static constexpr void SetFlag(int& current, TENUM flag1, TENUM flag2) {
		current |= static_cast<int>(flag1) | static_cast<int>(flag2);
	}

	template <typename TENUM>
	static constexpr void UnsetFlag(int& current, TENUM flag) {
		current &= ~static_cast<int>(flag);
	}

	template <typename TENUM>
	static constexpr void FlipFlag(int& current, TENUM flag) {
		current ^= static_cast<int>(flag);
	}

	template <typename TENUM>
	static constexpr bool HasFlag(int const& current, TENUM flag) {
		return (current & static_cast<int>(flag)) == static_cast<int>(flag);
	}

	template <typename TENUM>
	static constexpr bool HasAnyFlag(int const& current, TENUM flag) {
		return (current & static_cast<int>(flag)) != 0;
	}

	//Performs a std::make_shared<TDERIVED> and converts to base class with reinterpret_pointer_cast<TBASE>
	template <typename TBASE, typename TDERIVED, class... _Types>
	static inline std::shared_ptr<TBASE> reinterpret_make_shared(_Types&&... _Args) {
		auto derived = std::make_shared<TDERIVED>(std::forward<_Types>(_Args)...);
		return reinterpret_pointer_cast<TBASE>(derived);
	}

	//
	// Smart Pointer Comparator
	//

	template<typename T> struct is_shared_ptr : std::false_type {};
	template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
	template<typename T> struct is_weak_ptr : std::false_type {};
	template<typename T> struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};
	template<typename T> struct is_unique_ptr : std::false_type {};
	template<typename T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};

	//Returns true if the type is a smart pointer
	template <typename T>
	static constexpr bool IsSmartPointer() {
		return is_shared_ptr<T>::value || is_unique_ptr<T>::value || is_weak_ptr<T>::value;
	}

	// Convert a string to wstring
	static inline std::wstring ToWString(const std::string& str)
	{
		if (str.empty()) {
			return std::wstring();
		}

		// Passar nullptr no destino retorna a quantidade de wchar_t necessários
		size_t requiredSize = std::mbstowcs(nullptr, str.c_str(), 0);

		// Se a conversão falhar (caractere inválido), retorna string vazia
		if (requiredSize == static_cast<size_t>(-1)) {
			return std::wstring();
		}

		// Aloca exatamente o tamanho necessário
		std::wstring wstr(requiredSize, L'\0');

		// Executa a conversão real
		std::mbstowcs(&wstr[0], str.c_str(), requiredSize);

		return wstr;
	}

	// Convert a wstring to string
	static inline std::string ToString(const std::wstring& wstr)
	{
		if (wstr.empty()) {
			return std::string();
		}

		// Passar nullptr no destino retorna a quantidade de bytes (char) necessários
		size_t requiredSize = std::wcstombs(nullptr, wstr.c_str(), 0);

		if (requiredSize == static_cast<size_t>(-1)) {
			return std::string();
		}

		// Aloca exatamente o tamanho necessário
		std::string str(requiredSize, '\0');

		// Executa a conversão real
		std::wcstombs(&str[0], wstr.c_str(), requiredSize);

		return str;
	}

	//Returns a hash reporting input values
	template <class T>
	static constexpr void HashCombine(std::size_t& seed, const T& v) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

#define SOURCE_LOCATION std::source_location const& location = std::source_location::current()

	//Returns null if the type is a smart pointer or default value if the type has a default constructor.
	//Throws an exception if the object cannot be created
	template<typename T>
	static inline auto ReturnDefaultOrNull() {
		if constexpr (IsSmartPointer<T>() || std::is_pointer<T>::value)
			return (T)nullptr;
		else if constexpr (std::is_default_constructible<T>::value)
			return T();
		else if constexpr (std::is_constructible_v<T, std::nullptr_t>)
			return T(nullptr);		
	}

	static constexpr std::wstring Char32ToWString(char32_t c)
	{
		if constexpr (sizeof(wchar_t) == 2)
		{
			// UTF-16
			if (c <= 0xFFFF) {
				return std::wstring(1, static_cast<wchar_t>(c));
			}

			c -= 0x10000;
			wchar_t high = static_cast<wchar_t>((c >> 10) + 0xD800);
			wchar_t low = static_cast<wchar_t>((c & 0x3FF) + 0xDC00);

			return { high, low };
		}
		else
		{
			// UTF-32
			return std::wstring(1, static_cast<wchar_t>(c));
		}
	}	

	static constexpr bool safe_addition(size_t a, size_t b, size_t& result) {
		if (a > SIZE_MAX - b) {
			// Overflow ocorreria
			return false;
		}
		result = a + b;
		return true;
	}	

	template <typename T>
	static  void Erase(std::vector<T>& v, T const& item) {
		auto it = std::find(v.begin(), v.end(), item);

		if (it != v.end())
			v.erase(it);
	}

	template <typename T>
	static bool Contains(std::vector<T> const& v, T const& item) {
		return std::find(v.begin(), v.end(), item) != v.end();
	}
}