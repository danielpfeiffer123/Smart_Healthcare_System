#include "Session.h"
#include <QTcpSocket>
Session::Session(QTcpSocket* socket)//构造函数，初始化session对象
{
    m_socket = socket;//传入的套接字指针
    m_type = RECV_HEAD;//接收头部
    m_head = NULL;//头部数据指针
    m_bufLen = 0;//缓冲区长度
    m_readPos = 0;//读取位置
    m_body = NULL;//正文数据指针
    m_isFinish = false;//会话结束标志
    m_head = new char[sizeof(DeMessageHead)];//内存分配
    m_bufLen = sizeof(DeMessageHead);//缓冲区长度设置为头部长度
}

Session::~Session()//防止内存泄漏
{
    if (m_head != NULL)
        delete[] m_head; //头部指针不为空时释放内存
    if (m_body != NULL)
        delete[] m_body; //正文数据指针不为空时释放内存
    if (m_socket != NULL) //套接字不为空时关闭套接字并设置为空
	{
        m_socket->close();
        m_socket = NULL;
    }
};

int Session::readEvent() //读取
{
    int ret = 0;
    switch (m_type)
    {
    case RECV_HEAD: //读头部
        ret = recvHead(); 
        break;
    case RECV_BODY: //读数据正文
        ret = recvBody();
        break;
    default:
        break;
    }
    if (ret == RET_AGAIN) //需要再次读取
        return readEvent();
    return ret;
}

int Session::recvHead() //读取头部数据
{
    if (m_head == NULL) //重新分配内存
    {
        m_head = new char[sizeof(DeMessageHead)];
        assert(m_head != NULL); //确保内存重新分配成功
        m_bufLen = sizeof(DeMessageHead); //设置缓冲区长度
        m_readPos = 0; //初始化读取位置
    }
    int len = m_socket->read(m_head + m_readPos, m_bufLen - m_readPos);
    if (len < 0) //读取错误反馈
        return RET_ERROR;
    if (len == 0) //读取成功反馈
        return RET_END;
    m_readPos += len; //更新读取位置
    if (m_readPos == m_bufLen) //读取头部完成
    {
        m_type = RECV_BODY; //切换到接受正文状态
        int bufLen = ((DeMessageHead *)m_head)->length;//正文长度
        m_body = new char[bufLen]; //分配内存
        assert(m_body != NULL);//确保内存分配成功
        m_bufLen = bufLen;//设置缓冲区长度
        m_readPos = 0;  //读取的位置置零
        return RET_AGAIN; //传回需要重新读取，进入正文读取状态
    }
    return 0;
}

int Session::recvBody()//读取正文数据
{
    /* 先判断读取的位置是否是 ((DeMessageHead*)m_head)->length 接收头指定的长度 */
    if (m_readPos == m_bufLen)
    {
        m_type = RECV_HEAD; //切换回头部读取状态
        handleMsgBase(); //处理消息
        m_isFinish = true; //标记会话完成
        return RET_AGAIN; //准备下一次读取
    }
    /* 读取指定 Body 大小的数据 */
    int len = m_socket->read(m_body + m_readPos, m_bufLen - m_readPos);
    if (len < 0)
        return RET_ERROR; //读取失败反馈
    m_readPos += len; //更新读取位置
    /* 判断读取的位置是否是 ((DeMessageHead*)m_head)->length 接收头指定的长度 */
    if (m_readPos == m_bufLen)
    {
        m_type = RECV_HEAD;
        handleMsgBase();
        m_bufLen = 0;
        m_isFinish = true;
        return RET_AGAIN;
    }
    return RET_OK; //读取成功反馈
}

int Session::handleMsgBase() //处理接受的消息
{
    recvMsg *rMsg = new recvMsg(); //创建接收到的消息结构体
    rMsg->head = m_head; //消息头
    rMsg->body = m_body; //消息体
    rMsg->bodyLen = m_bufLen; //长度
    emit signal_handleMsg(rMsg); //发射信号，处理rMsg
    m_head = NULL;
    m_body = NULL;
    return RET_OK; //清空指针，处理成功反馈
}

void Session::cleanSession() //清理会话
{

}
