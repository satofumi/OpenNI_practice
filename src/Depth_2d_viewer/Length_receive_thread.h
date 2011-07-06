#ifndef LENGTH_RECEIVE_THREAD_H
#define LENGTH_RECEIVE_THREAD_H

/*!
  \file
  \brief 計測データの取得

  \author Satofumi KAMIMURA

  $Id$
*/

#include <QThread>
#include <memory>

namespace qrk
{
    class Lidar;
}


class Length_receive_thread : public QThread
{
    Q_OBJECT;

public:
    Length_receive_thread(qrk::Lidar& lidar);
    ~Length_receive_thread(void);

protected:
    void run(void);

private:
    Length_receive_thread(const Length_receive_thread& rhs);
    Length_receive_thread& operator = (const Length_receive_thread& rhs);

    struct pImpl;
    std::auto_ptr<pImpl> pimpl;
};

#endif /* !LENGTH_RECEIVE_THREAD_H */
