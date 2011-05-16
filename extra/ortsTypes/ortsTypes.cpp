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
char strOrtsTypes[21][255] = {
		{"type_ERRTYPE"},
		{"type_BYTE"},
		{"type_WORD"},
		{"type_CHAR"},
		{"type_DWORD"},
		{"type_FLOAT"},
		{"type_DOUBLE"},
		{"type_0x07"},
		{"type_QWORD"},
		{"type_0x09"},
		{"type_0x0A"},
		{"type_0x0B"},
		{"type_0x0C"},
		{"type_0x0D"},
		{"type_0x0E"},
		{"type_0x0F"},
		{"type_0x10"},
		{"type_BYTEVECTOR"},
		{"type_WORDVECTOR"},
		{"type_CHARVECTOR"},
		{"type_DWORDVECTOR"}
};

