class Converter {
public:
    virtual void process(WavFile& input, WavFile& output) = 0;
    virtual ~Converter() = default;
};
