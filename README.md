# data-transfer

Реализация клиент--серверного приложения. Клиент передаёт данные через сокет-tcp, сервер по схеме однопоточного неблокирующего tcp-сервера принимает и сохраняет данные в файл. 

Сборка:

    $ uname -r
    4.19.3-200.fc28.x86_64
    
    $ make
    
Запуск сервера:

    # ./transfer server
    
Запуск клиента:

    $ ./transfer data filename hostaddr
    
где data -- числовой ряд, filename -- фаил для записи на сервер и hostaddr -- ip системы с выполняемым модулем сервера.

Пример:

    $ ./transfer  1 2 3 5 7 8 9 path/dir/file.log 127.0.0.1
    
    transfer 3 sequence 5 path/dir/file.log to 127.0.0.1
    transfer 3 sequence 7 path/dir/file.log to 127.0.0.1


Для выполнения проверки может потребоваться разрешить на системе с выполняемым модулем сервера обработку, поступающих на 127 порт-tcp запросов. Для разрешения обработки: 

    # firewall-cmd --add-port=127/tcp --permanent
    
    # firewall-cmd --reload 
    
для отмены разрешения:

    # firewall-cmd --remove-port=127/tcp --permanent
    
    # firewall-cmd --reload 
