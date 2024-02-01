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
                }

                io = group "io" {
                    data_sender = component "DataSender" {
                        description "Receives mapped data and sends in over a stream protocol"
                        engine -> this "Report (mapped)"
                        tags "communication"  
                    }

                    data_receiver = component "DataReceiver" {
                        this -> engine "Data (mapped)"
                        tags "communication" 
                    }
                }

                tasks = group "tasks" {
                    task_factory = component "TaskFactory" {
                        description "Receives a mapped Task Configuration Data, creates corresponding task and returns it to the user"

                        engine -> this "Data (defines task)"
                        this -> engine "Task"                    
                        tags "factory"
                    }

                    inventory_task_factory_gpio = component "InventoryTaskFactoryGpio" {
                        task_factory -> this "Data"
                        this -> task_factory "Task"
                        tags "inventory factory"
                    }

                    inventory_task_factory_stepper_motor = component "InventoryTaskFactoryStepperMotor" {
                        task_factory -> this "Data"
                        this -> task_factory "Task"  
                        tags "inventory factory"
                    }

                }

                hardware_abstraction = group "hardware_abstraction" {
                    gpio = component "Gpio" {
                        inventory_task_factory_gpio -> this "uses"
                        inventory_task_factory_stepper_motor -> this "uses"
                        tags "hw abstraction" 
                    }
                    timer = component "Timer" {
                        inventory_task_factory_stepper_motor -> this "uses"
                        tags "hw abstraction" 
                    }
                    uart = component "Uart" {
                        data_receiver -> this "uses"
                        data_sender -> this "uses"
                        tags "hw abstraction" 
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
            element "inventory factory" {
                background "blue"
                color "white"
                shape RoundedBox
            }
            element "communication" {
                background "orange"
                color "white"
                shape RoundedBox
            }
            element "hw abstraction" {
                background "green"
                color "white"
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