# SY5 - PROJET SYSTEMES

A Simulation of a printers System.

## Compilation

    make

## To Execute

To start running the printers simulators you need to execute the following command, which will read inside the file `printers_list` in the project directory, where each line is of the format `printer_name printer_tube\n` 

	./init_simulateurs -c printers_list

The last command will execute as many printers as is the number of lines in the configuration file.
To start the Server you need to specify through which **pipe** it will wait for incoming requests and how to communicate with printers (-c option):

    ./print_demon -t server_tube -c printers_list

To send a simple printing request, execute the client with the "-P" option:

    ./mpr -P printer_name file_to_print client_tube

To cancel a printing requests:

    ./mpr -P printer_name file_to_print tube_client -A printing_id

To send N printing requests for the file `file_to_print`:

    ./mpr -P printer_name file_to_print client_tube -N number_of_copies

To print all the files specified in `files_list` (a file in which each line is a path to the file to print):

    ./mpr -P printer_name file_to_print client_tube -M file_list

## Synopsis 

The project is a Simulation of a System where clients can ask to print a file through a specific printer. The central Server keeps listening for incoming printing requests and distributes them to the right printer, which add the new file to print in a queue.  

The Client sends a printing request to the Server and receives an ID in response, then waits for the Job to be done by the printer.
The client can still stop a file from printing by sending a request with "-A" option, if the job hasn't been treated by the printer yet.

The Server-Client and Server-Printers communication takes place through **pipes**, but it could be easily replaced by real network communication to make this a distributed system.

## Tests 

Functional and Unit Tests have been written to verify if the system functionnalities correctly work. You can easily start the tests with the following command:

	sudo ./test.sh

The script will launch both functional and unity tests. The first ones are obtained by executing a series of **.sh** scripts contained in `Tests/fonctionnels/bad` and `Tests/fonctionnels/good`. This way the number of functional tests can be increased just by adding the new test script in the right directory, the system will automatically consider the new test inserted. Unit Tests use **CUnit library**, so that if you want to add tests, you need to modify `Tests/unitaires/test_cunit.c` file.
