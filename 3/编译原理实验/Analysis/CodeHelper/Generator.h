

#ifndef ANALYSIS_GENERATOR_H
#define ANALYSIS_GENERATOR_H

class Generator {
public:
    int id = 0;

    int get() {
        return id++;
    }
};

#endif //ANALYSIS_GENERATOR_H
