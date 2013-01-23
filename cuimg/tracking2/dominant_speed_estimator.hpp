#ifndef CUIMG_DOMINANT_SPEED_HPP_
# define CUIMG_DOMINANT_SPEED_HPP_

# include <cuimg/improved_builtin.h>
# include <cuimg/cpu/fill.h>

namespace cuimg
{

  dominant_speed_estimator::dominant_speed_estimator(const obox2d& d)
    : h(d / 4)
  {
  }

  dominant_speed_estimator::dominant_speed_estimator(const dominant_speed_estimator& d)
    : h(d.h.domain())
  {
    copy(d.h, h);
  }

  // dominant_speed_estimator&
  // dominant_speed_estimator::operator=(const dominant_speed_estimator& d)
  // {
  //   h = host_image2d<unsigned short>(d.h.domain());
  //   copy(d.h, h);
  //   return *this;
  // }

  template <typename PI>
  i_short2
  dominant_speed_estimator::estimate(const PI& pset, i_int2 prev_camera_motion)
  {
    SCOPE_PROF(dominant_speed_estimation);

    typedef unsigned short US;
    fill(h, US(0));
    i_int2 h_center(h.nrows() / 2, h.ncols() / 2);
    int max = 0;
    i_int2 max_bin = h_center;
    for (unsigned i = 0; i < pset.dense_particles().size(); i++)
    {
      const particle& part = pset[i];
      i_int2 bin = h_center + part.acceleration + prev_camera_motion;
      if (part.age > 2 and h.has(bin))
      {
        int c = ++h(bin);
        if (c > max)
        {
          max = c;
          max_bin = bin;
        }
      }
      // else if (part.age == 1)
      // {
      //   int c = ++h(h_center + part.speed);
      //   if (c > max)
      //   {
      //     max = c;
      //     max_bin = bin;
      //   }
      // }
    }

    return max_bin - h_center;
  }

}

#endif
