char strErrTypes[13][255]=
       {{"Успешно"},			//
       {"Ошибка исполнения"},
       {"Ошибка контрольной суммы"},
       {"Инициализация не была проведена"},
       {"Ошибка декодирования параметров"},
       {"Ошибка значения параметров"},
       {"Расширенный ответ"},
       {"Истекло время ожидания"},
       {"Выполнение прервано"},
       {"Ошибка выделения памяти"},
       {"Ошибка доступа к сокету"},
       {"Не найден"},
       {"Операция не разрешена"}};

char lenOrtsTypes[21]={
		1,//size of element of type_ERRTYPE
		1,//size of element of type_BYTE
		2,//size of element of type_WORD
		1, //size of element of type_CHAR
		4, //size of element of type_DWORD
		4, //size of element of type_FLOAT
		8, //size of element of type_DOUBLE
		0,
		8, //size of element of type_QWORD
		0,
		0,0,0,0,0,0,0, //10-16:
		1, //17: size of element of type_BYTEVECTOR
		2, //18: size of element of type_WORDVECTOR
		1, //19: size of element of type_CHARVECTOR;
		4};//20: size of element of type_DWORDVECTOR;
