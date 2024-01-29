#include "data.hpp"
#include "engine.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "inventory_task_factory.hpp"
#include "object.hpp"
#include "task.hpp"
#include "task_factory.hpp"
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

using GpioInventory = Inventory<int, Gpio>;
using GpioTaskFactory = InventoryTaskFactory<int, Gpio>;

int main(void) {
    GpioInventory gpio_inventory;
    
    GpioTaskFactory task_factory(
        &gpio_inventory,
        [](const Data& data)-> GpioTaskFactory::TaskType {
            return static_cast<GpioTaskFactory::TaskType>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
        }
    );
    task_factory.set_creator(
        GpioTaskFactory::TaskType::CREATE_INVENTORY_ITEM,
        [&](GpioInventory *inventory, const Data& cfg)-> Task * {
            return new InventoryTask<int, Gpio> (
                inventory,
                [&](GpioInventory *gpio_inv, const data::Data& cfg_data)-> void {
                    auto id = Data::cast<Integer>(Data::cast<Object>(cfg_data).access("id")).get();
                    auto dir = static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(cfg_data).access("direction")).get());
                    gpio_inv->put(id, std::shared_ptr<Gpio>(new Gpio(id, dir)));
                },
                cfg
            );
        }
    );
    task_factory.set_creator(
        GpioTaskFactory::TaskType::DELETE_INVENTORY_ITEM,
        [&](GpioInventory *inventory, const Data& cfg)-> Task * {
            return new InventoryTask<int, Gpio> (
                inventory,
                [&](GpioInventory *gpio_inv, const data::Data& cfg_data)-> void {
                    auto id = Data::cast<Integer>(Data::cast<Object>(cfg_data).access("id")).get();
                    gpio_inv->remove(id);
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