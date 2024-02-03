workspace {

    model {
        user = person "User" {
            description "Defines a program which should be executed by the hardware as a sequence of tasks"
        }
        
        cnc_controller = softwareSystem "Cnc Controller" {
            description "a bound of CNC Server running on the CNC hardware and CNC Client running on the host"

            offboard_client = container "Offboard Client" {
                description "Parses received from the User tasks, sends them one-after-other to the Onboard server"

                user -> this "Tasks *.json file"
                this -> user "Report *.json file"
            }

            onboard_server = container "Onboard Server" {
                description "Parses received from the User tasks, sends them one-after-other to the Onboard server"

                bl = group "bl" {
                    engine = component "Engine" {
                        description "Receives a mapped Task Configuration Data, creates it with TaskFactory, executes it, composes a report to the Client and send it over the ReportSender"
                    }

                    factory = component "Factory" {
                        description "An interface of abstract factory receiving Data and producing Tasks"
                        engine -> this "uses"
                        tags "factory"
                    }

                    task = component "Task" {
                        description "An interface of abstract executable and reportable task"
                        engine -> this "executes"
                        factory -> this "produces"
                        tags "task"
                    }

                    data = component "Data" {
                        description "Data structures used by BL to communicate with other entities"
                        engine -> this "uses"
                        tags "data"
                        factory -> this "uses"
                    }

                    reporter = component "Reporter" {
                        description "An interface of abstract communication class used to send a report about task execution to the offboard client"
                        engine -> this "uses"
                        tags "communication"
                    }
                }

                io = group "io" {
                    report_sender = component "Report Sender" {
                        description "Receives mapped data and sends in over a stream protocol"
                        this -> reporter "implements"
                        tags "communication"  
                    }

                    package_receiver = component "Package Receiver" {
                        description "Accumulates incomming stream data and dispatches packages"
                        tags "communication"
                    }

                    data_parser = component "Data Parser" {
                        description "Parses incomming packages into Data objects"
                        this -> data "uses"
                        tags "communication"
                    }

                    data_dispatcher = component "Data Dispatcher" {
                        description "Parses incomming packages into Data objects"
                        this -> package_receiver "uses"
                        this -> data_parser "uses"
                        this -> engine "dispatches"
                        tags "communication"
                    }
                }

                tasks_factory = group "factory" {
                    task_factory = component "Task Factory" {
                        description "Receives a mapped Task Configuration Data, creates corresponding task and returns it to the user"
                        this -> factory "implements"                    
                        this -> data "uses"
                        tags "factory"
                    }
                }

                mcu_control = group "mcu_control" {
                    gpio_inventory = component "Gpio Inventory" {
                        description "Stores and provides access to GPIOs"
                        tags "mcu_control"
                    }

                    timer_inventory = component "Timer Inventory" {
                        description "Stores and provides access to Timers"
                        tags "mcu_control"
                    }

                    uart_inventory = component "Uart Inventory" {
                        description "Stores and provides access to Uarts"
                        tags "mcu_control"
                    }

                    gpio = component "Gpio" {
                        gpio_inventory -> this "stores"
                        tags "mcu_control" 
                    }

                    timer = component "Timer" {
                        timer_inventory -> this "stores"
                        tags "mcu_control" 
                    }

                    uart = component "Uart" {
                        uart_inventory -> this "stores"
                        tags "mcu_control" 
                    }

                    mcu_control_facade = component "McuControl" {
                        this -> gpio_inventory "composes"
                        this -> timer_inventory "composes"
                        this -> uart_inventory "composes"
                        task_factory -> this "aggregates"
                        report_sender -> this "uses"
                        package_receiver -> this "uses"
                        tags "mcu_control"
                    }

                }
            }
        }

        cnc_hardware = group "cnc_hardware" {
            mcu_timer = element "McuTimer" {
                timer -> this "controls"
                tags "mcu hardware"
            }

            mcu_gpio = element "McuGpio" {
                gpio -> this "controls"
                tags "mcu hardware"
            }

            mcu_uart = element "McuUart" {
                offboard_client -> this "Data (byte stream)"
                this -> offboard_client "Report (byte stream)"
                this -> uart "Data (byte stream)"
                uart -> this "Report (byte stream)"
                tags "mcu hardware"
            }
        }
    }

    views {
        sys_context = systemContext cnc_controller "SystemContext" {
            include *
            autoLayout
        }

        container cnc_controller "Containers" {
            include *
            autoLayout
        }

        component onboard_server "OnboardServerComponents" {
            include *
            include offboard_client
            include user
            autoLayout
        }

        styles {
            element "Person" {
                background "green"
                color "white"
                shape Person
            }
            element "Software System" {
                background "blue"
                color "white"
                shape RoundedBox
            }
            element "factory" {
                background "blue"
                color "white"
                shape RoundedBox
            }
            element "inventory" {
                background "magenta"
                color "black"
                shape RoundedBox
            }
            element "data" {
                background "pink"
                color "black"
                shape RoundedBox
            }
            element "task" {
                background "violet"
                color "black"
                shape RoundedBox
            }
            element "communication" {
                background "orange"
                color "black"
                shape RoundedBox
            }
            element "hw abstraction" {
                background "green"
                color "white"
                shape RoundedBox
            }
            element "mcu_control" {
                background "violet"
                color "black"
                shape RoundedBox
            } 
            element "mcu hardware" {
                background "yellow"
                color "black"
                shape Hexagon
            }            
        }
    }
    
}