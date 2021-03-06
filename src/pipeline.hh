/* pipeline.hh: Generate pipeline */
#ifndef _PIPELINE_HH_
#define _PIPELINE_HH_

using namespace cv;

tbb::pipeline create_pipeline(void);
Mat takeOneFrame(VideoCapture& cap);

class Initializer : public tbb::filter
{
  public:
    Initializer()
  : tbb::filter(tbb::filter::serial_in_order),
    cap_(0)
  {}

    ~Initializer()
  {
      cap_.release();
  }

  // Generate tokens
  void* operator()(void*) override
  {
      Mat* picture = new Mat(takeOneFrame(cap_));
      return (void*) picture;
  }

  private:
    VideoCapture cap_;
};

class Terminator : public tbb::filter
{
  public:
    Terminator()
  : tbb::filter(tbb::filter::serial_in_order) {}

  // Process tokens
  void* operator()(void* token) override
  {
      Mat* picture = (Mat*) token;
      imshow("Picture", *picture);
      delete picture;
      return nullptr;
  }
};

#endif /* _PIPELINE_HH_ */
