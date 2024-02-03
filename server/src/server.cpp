#include <iostream>
#include <memory>
#include <stdexcept>

#include "data.hpp"
#include "engine.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "task_factory.hpp"
#include "object.hpp"
#include "sender.hpp"

using namespace engine;
using namespace task_factory;
using namespace data;
using namespace inventory;
using namespace tasks;

class ReportSender: public Sender {
public:
    virtual void send(const Data& data) const {
        std::cout << "report: result = " << std::endl;
    }
};
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

static TaskFactory get_task_factory();
static Object get_create_gpio_task_data(int id, const Gpio::Direction& dir);
static Object get_set_gpio_task_data(int id, const Gpio::Value& val);
static Object get_delete_gpio_task_data(int id);

static Inventory<int, Gpio> s_gpio_inventory;

int main(void) {
    TaskFactory mcu_task_factory(get_task_factory());
    ReportSender sender;
    Engine engine(mcu_task_factory, sender);

    int gpio_num = 12;

    engine.run_task(get_create_gpio_task_data(gpio_num, Gpio::Direction::INPUT));

    if (!s_gpio_inventory.contains(gpio_num)) {
        throw std::runtime_error("create task failed");
    }

    engine.run_task(get_set_gpio_task_data(gpio_num, Gpio::Value::HIGH));

    engine.run_task(get_delete_gpio_task_data(gpio_num));

    if (s_gpio_inventory.contains(gpio_num)) {
        throw std::runtime_error("delete task failed");
    }

    return 0;
}

// static functions/classes implementations

inline Object get_create_gpio_task_data(int id, const Gpio::Direction& dir) {
    Object data;
    data.add("task_type", String("create_gpio"));
    data.add("gpio_id", Integer(id));
    data.add("gpio_dir", Integer(static_cast<int>(dir)));
    return data;
}

inline Object get_set_gpio_task_data(int id, const Gpio::Value& val) {
    Object data;
    data.add("task_type", String("set_gpio"));
    data.add("gpio_id", Integer(id));
    data.add("gpio_val", Integer(static_cast<int>(val)));
    return data;
}

inline Object get_delete_gpio_task_data(int id) {
    Object data;
    data.add("task_type", String("delete_gpio"));
    data.add("gpio_id", Integer(id));
    return data;
}

inline int get_gpio_id(const Data& data) {
    return Data::cast<Integer>(Data::cast<Object>(data).access("gpio_id")).get();
}

inline Gpio::Direction get_gpio_dir(const Data& data) {
    return static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_dir")).get());
}

inline Gpio::Value get_gpio_val(const Data& data) {
    return static_cast<Gpio::Value>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_val")).get());
}

inline TaskFactory get_task_factory() {
    TaskFactory factory(
        [](const Data& cfg)-> std::string {
            return Data::cast<String>(Data::cast<Object>(cfg).access("task_type")).get();
        }
    );

    factory.set_creator(
        "create_gpio",
        [&](const Data& cfg) {
            return new InventoryTask<int, Gpio>(
                &s_gpio_inventory,
                [](Inventory<int, Gpio> *inventory_ptr, const Data& cfg) {
                    inventory_ptr->put(get_gpio_id(cfg), std::shared_ptr<Gpio>(new Gpio(get_gpio_id(cfg), get_gpio_dir(cfg))));
                },
                cfg
            );
        }
    );

    factory.set_creator(
        "set_gpio",
        [&](const Data& cfg) {
            return new InventoryTask<int, Gpio>(
                &s_gpio_inventory,
                [](Inventory<int, Gpio> *inventory_ptr, const Data& cfg) {
                    (inventory_ptr->get(get_gpio_id(cfg)))->set(get_gpio_val(cfg));
                },
                cfg
            );
        }
    );

    factory.set_creator(
        "delete_gpio",
        [&](const Data& cfg) {
            return new InventoryTask<int, Gpio>(
                &s_gpio_inventory,
                [](Inventory<int, Gpio> *inventory_ptr, const Data& cfg) {
                    inventory_ptr->remove(get_gpio_id(cfg));
                },
                cfg
            );
        }
    );
    return factory;
}