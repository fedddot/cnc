#include "data.hpp"
#include "delete_inventory_item_task.hpp"
#include "engine.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "object.hpp"
#include "task.hpp"
#include "task_factory.hpp"
#include "create_inventory_item_task.hpp"
#include <iostream>

using namespace cnc_engine;
using namespace task_factory;
using namespace data;
using namespace basics;
using namespace inventory;
using namespace tasks;

class Gpio {
public:
    enum class Direction: int {
        INPUT,
        OUTPUT
    };
    enum class Value: int {
        LOW,
        HIGH
    };
    Gpio(int id, const Direction& direction): m_id(id), m_direction(direction), m_val(Value::LOW) {

    }
    Value get() const {
        return m_val;
    }
    void set(const Value& val) {
        m_val = val;
    }
private:
    int m_id;
    Direction m_direction;
    Value m_val;
};

class ReportSender: public Engine::ReportSender {
public:
    virtual void send(const Report& data) const {
        std::cout << "report: result = " << static_cast<int>(data.result()) << std::endl;
    }
};

int main(void) {
    Inventory<int, Gpio> gpio_inventory;
    
    TaskFactory task_factory;
    auto gpio_creator = [](const Data& cfg)-> Gpio * {
        const Object& cfg_data = Data::cast<Object>(cfg);
        Gpio::Direction gpio_dir = static_cast<Gpio::Direction>(Data::cast<Integer>(cfg_data.access("direction")).get());
        int gpio_id = Data::cast<Integer>(cfg_data.access("id")).get();
        return new Gpio(gpio_id, gpio_dir);
    };

    task_factory.set_creator(
        TaskFactory::TaskType::CREATE_INVENTORY_ITEM,
        [&](const Data& cfg)-> Task * {
            int gpio_id = Data::cast<Integer>(Data::cast<Object>(cfg).access("id")).get();
            return new CreateInventoryItemTask<int, Gpio>(
                gpio_inventory, 
                gpio_id, 
                cfg, 
                gpio_creator
            );
        }
    );

    task_factory.set_creator(
        TaskFactory::TaskType::DELETE_INVENTORY_ITEM,
        [&](const Data& cfg)-> Task * {
            int gpio_id = Data::cast<Integer>(Data::cast<Object>(cfg).access("id")).get();
            return new DeleteInventoryItemTask<int, Gpio>(
                gpio_inventory, 
                gpio_id
            );
        }
    );

    ReportSender sender;
    Engine engine(task_factory, sender);

    Object taskCfg;
    taskCfg.add("task_type", Integer(static_cast<int>(TaskFactory::TaskType::CREATE_INVENTORY_ITEM)));
    taskCfg.add("id", Integer(0));
    taskCfg.add("direction", Integer(static_cast<int>(Gpio::Direction::INPUT)));


    engine.run_task(taskCfg);

    return 0;
}