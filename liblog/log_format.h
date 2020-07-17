#ifndef __LOG_FORMAT_H__
#define __LOG_FORMAT_H__

#include <vector>
#include <iterator>

#define LOG_FORMAT_DEFAULT (infra::LogFormat() << infra::LogFormat::Field::LeftMidBrackets \
                                               << infra::LogFormat::Field::Year \
                                               << infra::LogFormat::Field::HorizontalLine \
                                               << infra::LogFormat::Field::Month \
                                               << infra::LogFormat::Field::HorizontalLine \
                                               << infra::LogFormat::Field::DayOfMonth \
                                               << infra::LogFormat::Field::Blank \
                                               << infra::LogFormat::Field::Hour \
                                               << infra::LogFormat::Field::Colon \
                                               << infra::LogFormat::Field::Minute \
                                               << infra::LogFormat::Field::Colon \
                                               << infra::LogFormat::Field::Second \
                                               << infra::LogFormat::Field::Dot \
                                               << infra::LogFormat::Field::Microsecond \
                                               << infra::LogFormat::Field::RightMidBrackets \
                                               << infra::LogFormat::Field::Blank \
                                               << infra::LogFormat::Field::Logschema \
                                               << infra::LogFormat::Field::Colon \
                                               << infra::LogFormat::Field::Blank)

namespace infra {

class LogFormat {
public:
    enum class Field : int {
        Pid,
        Tid,
        Function,
        File,
        LineNo,
        TimeStamp,
        Year,
        Month,
        DayOfMonth,
        Week,
        DayOfWeek,
        Hour,
        Minute,
        Second,
        Millisecond,
        Microsecond,
        Nanosecond,

        Blank,
        Dot,
        Colon,
        LeftBigBrackets,
        RightBigBrackets,
        LeftMidBrackets,
        RightMidBrackets,
        LeftSmallBrackets,
        RightSmallBrackets,
        HorizontalLine,
        VerticalLine,

        Logschema,
    };
public:
    class iterator : public std::iterator<std::input_iterator_tag, std::vector<Field>::iterator> {
    public:
        friend class LogFormat;
        iterator(std::vector<Field>::iterator&& vit) : ptr(vit) { }
        iterator() { }

        iterator(const iterator& other) : ptr(other.ptr) { }
        ~iterator() { }


        Field& operator*() { return *(ptr); }
        bool operator!=(const iterator& other) const { return (ptr != other.ptr); }
        bool operator==(const iterator& other) const { return (ptr == other.ptr); }
        const iterator& operator--() {
            ptr--;
            return *this;
        }
        const iterator operator--(int) {
            --ptr;
            return *this;
        }
        const iterator& operator++() {
            ptr++;
            return *this;
        }
        const iterator operator++(int) {
            ++ptr;
            return *this;
        }

    private:
        std::vector<Field>::iterator ptr;
    };
public:
    LogFormat() { }
    ~LogFormat() { }

    iterator begin () { return iterator(field_vec_.begin()); }
    iterator end () { return iterator(field_vec_.end()); }

    LogFormat& operator<<(Field&& field) {
        field_vec_.push_back(field);
        return *this;
    }

    LogFormat& operator>>(Field&& field) {
        field = field_vec_.back();
        field_vec_.pop_back();
        return *this;
    }
    LogFormat& operator>>(Field& field) {
        field = field_vec_.back();
        field_vec_.pop_back();
        return *this;
    }

public:
    std::vector<Field> field_vec_;
};

}

#endif
