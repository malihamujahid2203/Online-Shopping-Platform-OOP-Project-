#ifndef discount_h
#define discount_h

class Discount {
public:
    virtual double apply(double subtotal) const = 0;
    virtual ~Discount() {}
};

class NoDiscount : public Discount {
public:
    double apply(double subtotal) const override {
        return 0;
    }
};

class PercentageDiscount : public Discount {
private:
    double percent;
public:
    PercentageDiscount(double p) {
        percent = p;
    }

    double apply(double subtotal) const override {
        return subtotal * percent / 100.0;
    }
};

#endif
