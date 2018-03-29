/** @addtogroup TextToTabEditor
 * @{*/

/** @file */

#ifndef FORMISTAKES_GASPARYANMOSES
#define FORMISTAKES_GASPARYANMOSES

#include <sstream>
#include <iostream>

namespace fttw{

/*! \~russian
 * \brief Преобразует элемент на входе в строку.
 * \details У элемента должен быть определен оператор вывода,
 * т.к. преобразование происходит за счет простого вывода в строковый поток.
 * \param elem параметр, который будет преобразован в строку.
 * \return строку, состоящую из входного параметра.
 */
template<typename T>
inline std::string toStr(T elem) {
	std::stringstream vss;
	vss<<elem;
	return vss.str();
}

/*! \~russian
 * \brief Печатает в поток ошибок сообщение об ошибке.
 * \details Печатает в поток ошибок сообщение об ошибке с указанием функции,
 * в которой произошла ошибка и номер строки,
 * на которой произошла ошибка и имя файла, а затем выходит из программы с помощью throw.
 * \param funcName имя функции, которое будет выведено (часто используется макрос __FUNCTION__ с дополнительным указанием аргументов).
 * \param line номер строки, на которой вызвана эта функция (зачастую достаточно использовать макрос __LINE__).
 * \param fileName имя файла, в котором находится функция, из которой произошел вызов функции-ошибки (зачастую достаточно использовать макрос __FILE__).
 * \param message сообщение об ошибке. Должно быть введено вручную.
 */
inline void print_mistake(const std::string& funcName, int line, const std::string& fileName, const std::string& message){
	std::cerr<<"ERROR in "<<funcName<<" : line "<<line<<", file "<<fileName<<std::endl;
	std::cerr<<"\t"<<message<<std::endl;
	throw;
}

/*! \~russian
 * \brief Печатает в поток ошибок сообщение о предупреждении.
 * \details Печатает в поток ошибок сообщение о предупреждении с указанием функции,
 * в которой произошла ошибка и номер строки,
 * на которой произошла ошибка и имя файла. Не завершает работу программы.
 * \param funcName имя функции, которое будет выведено (часто используется макрос __FUNCTION__ с дополнительным указанием аргументов).
 * \param line номер строки, на которой вызвана эта функция (зачастую достаточно использовать макрос __LINE__).
 * \param fileName имя файла, в котором находится функция, из которой произошел вызов данной функции (зачастую достаточно использовать макрос __FILE__).
 * \param message сообщение-предупреждение. Должно быть введено вручную.
 */
inline void print_mistakeLite(const std::string& funcName, int line, const std::string& fileName, const std::string& message) {
	std::cerr<<"WARNING in "<<funcName<<" : line "<<line<<", file "<<fileName<<std::endl;
	std::cerr<<"\t"<<message<<std::endl;
}

} // namespace fttw

#endif // FORMISTAKES_GASPARYANMOSES

/*@}*/
