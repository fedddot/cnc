#ifndef	__JSON_TYPES_HPP__
#define	__JSON_TYPES_HPP__

namespace json {
	enum class JsonSpecialChar : char {
		STRING_START = '\"',
		STRING_END = '\"',
		ARRAY_START = '[',
		ARRAY_END = ']',
		OBJECT_START = '{',
		OBJECT_END = '}',
		DELIMITER = ',',
		SEMICOLON = ':',
		SPACE = ' ',
		TAB = '\t',
		NEW_LINE = '\n'
	};
}

#endif // __JSON_TYPES_HPP__