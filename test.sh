clear
echo Для конфигурирования ввода, используется hexString.input
echo -
echo Создание бинарного файла - Response.dat
./hex_to_binary
echo Разбор полученной информации в таблицу.
./tlv_application_parser