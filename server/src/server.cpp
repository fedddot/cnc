#include "data.hpp"
#include "delete_inventory_item_task.hpp"
#include "engine.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "object.hpp"
#include "task.hpp"
#include "task_factory.hpp"
#include "create_inventory_item_task.hpp"
#include <iostream>
#include <stdexcept>

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
            return new InventoryTask<int, Gpio>(
                &gpio_inventory,
                [&](Inventory<int, Gpio> *inventory, const Data& data)-> void {
                    const Object& data_object = Data::cast<Object>(data);
                    int id = Data::cast<Integer>(data_object.access("id")).get();
                    Gpio::Direction dir = static_cast<Gpio::Direction>(Data::cast<Integer>(data_object.access("direction")).get());
                    inventory->put(id, std::shared_ptr<Gpio>(new Gpio(id, dir)));
                },
                cfg
            );
        }
    );

    task_factory.set_creator(
        TaskFactory::TaskType::DELETE_INVENTORY_ITEM,
        [&](const Data& cfg)-> Task * {
            return new InventoryTask<int, Gpio>(
                &gpio_inventory,
                [&](Inventory<int, Gpio> *inventory, const Data& data)-> void {
                    const Object& data_object = Data::cast<Object>(data);
                    int id = Data::cast<Integer>(data_object.access("id")).get();
                    inventory->remove(id);
                },
                cfg
            );
        }
    );

    ReportSender sender;
    Engine engine(task_factory, sender);

    Object createTaskCfg;
    createTaskCfg.add("task_type", Integer(static_cast<int>(TaskFactory::TaskType::CREATE_INVENTORY_ITEM)));
    createTaskCfg.add("id", Integer(0));
    createTaskCfg.add("direction", Integer(static_cast<int>(Gpio::Direction::INPUT)));
    engine.run_task(createTaskCfg);

    if (!gpio_inventory.contains(0)) {
        throw std::runtime_error("create task failed");
    }

    Object deleteTaskCfg;
    deleteTaskCfg.add("task_type", Integer(static_cast<int>(TaskFactory::TaskType::DELETE_INVENTORY_ITEM)));
    deleteTaskCfg.add("id", Integer(0));
    engine.run_task(deleteTaskCfg);

    if (gpio_inventory.contains(0)) {
        throw std::runtime_error("delete task failed");
    }

    return 0;
}