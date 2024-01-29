#include "data.hpp"
#include "engine.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "inventory_task_factory.hpp"
#include "object.hpp"
#include "task.hpp"
#include <iostream>
#include <stdexcept>

using namespace cnc_engine;
using namespace task_factory;
using namespace data;
using namespace basics;
using namespace inventory;
using namespace tasks;

class Gpio;

class ReportSender: public Engine::ReportSender {
public:
    virtual void send(const Report& data) const {
        std::cout << "report: result = " << static_cast<int>(data.result()) << std::endl;
    }
};

using GpioInventory = Inventory<int, Gpio>;
using GpioTaskFactory = InventoryTaskFactory<int, Gpio>;

static GpioTaskFactory createGpioTaskFactory(GpioInventory *inventory_ptr);
static Object getCreateGpioTaskData(int id);
static Object getDeleteGpioTaskData(int id);
static Object getSetGpioTaskData(int id);

int main(void) {
    GpioInventory gpio_inventory;    
    GpioTaskFactory task_factory(createGpioTaskFactory(&gpio_inventory));

    ReportSender sender;
    Engine engine(task_factory, sender);

    int gpio_num = 12;

    engine.run_task(getCreateGpioTaskData(gpio_num));

    if (!gpio_inventory.contains(gpio_num)) {
        throw std::runtime_error("create task failed");
    }

    engine.run_task(getSetGpioTaskData(gpio_num));

    engine.run_task(getDeleteGpioTaskData(gpio_num));

    if (gpio_inventory.contains(gpio_num)) {
        throw std::runtime_error("delete task failed");
    }

    return 0;
}

// static functions implementations

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

inline Object getCreateGpioTaskData(int id) {
    Object data;
    data.add("task_type", Integer(static_cast<int>(GpioTaskFactory::TaskType::CREATE_INVENTORY_ITEM)));
    data.add("id", Integer(id));
    data.add("direction", Integer(static_cast<int>(Gpio::Direction::INPUT)));
    return data;
}

inline Object getDeleteGpioTaskData(int id) {
    Object data;
    data.add("task_type", Integer(static_cast<int>(GpioTaskFactory::TaskType::DELETE_INVENTORY_ITEM)));
    data.add("id", Integer(id));
    return data;
}

inline Object getSetGpioTaskData(int id) {
    Object data;
    data.add("task_type", Integer(static_cast<int>(GpioTaskFactory::TaskType::USE_INVENTORY_ITEM)));
    data.add("id", Integer(id));
    data.add("value", Integer(static_cast<int>(Gpio::Value::HIGH)));
    return data;
}

inline void create_gpio_action(GpioInventory *gpio_inv, const data::Data& cfg_data) {
    auto id = Data::cast<Integer>(Data::cast<Object>(cfg_data).access("id")).get();
    auto dir = static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(cfg_data).access("direction")).get());
    gpio_inv->put(id, std::shared_ptr<Gpio>(new Gpio(id, dir)));
}

inline void delete_gpio_action(GpioInventory *gpio_inv, const data::Data& cfg_data) {
    auto id = Data::cast<Integer>(Data::cast<Object>(cfg_data).access("id")).get();
    gpio_inv->remove(id);
}

inline void set_gpio_action(GpioInventory *gpio_inv, const data::Data& cfg_data) {
    auto id = Data::cast<Integer>(Data::cast<Object>(cfg_data).access("id")).get();
    auto value = static_cast<Gpio::Value>(Data::cast<Integer>(Data::cast<Object>(cfg_data).access("value")).get());
    gpio_inv->get(id)->set(value);
}

inline GpioTaskFactory createGpioTaskFactory(GpioInventory *inventory_ptr) {
    GpioTaskFactory factory(
        inventory_ptr,
        [](const Data& data)-> GpioTaskFactory::TaskType {
            return static_cast<GpioTaskFactory::TaskType>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
        }
    );
    factory.set_creator(
        GpioTaskFactory::TaskType::CREATE_INVENTORY_ITEM,
        [&](GpioInventory *inventory, const Data& cfg)-> Task * {
            return new InventoryTask<int, Gpio> (
                inventory,
                create_gpio_action,
                cfg
            );
        }
    );
    factory.set_creator(
        GpioTaskFactory::TaskType::DELETE_INVENTORY_ITEM,
        [&](GpioInventory *inventory, const Data& cfg)-> Task * {
            return new InventoryTask<int, Gpio> (
                inventory,
                delete_gpio_action,
                cfg
            );
        }
    );
    factory.set_creator(
        GpioTaskFactory::TaskType::USE_INVENTORY_ITEM,
        [&](GpioInventory *inventory, const Data& cfg)-> Task * {
            return new InventoryTask<int, Gpio> (
                inventory,
                set_gpio_action,
                cfg
            );
        }
    );
    return factory;
}