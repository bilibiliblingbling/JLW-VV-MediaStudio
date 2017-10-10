#ifndef KV_MS_TMP_LOG_H
#define KV_MS_TMP_LOG_H

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdarg>

#if !defined(NDEBUG) && !defined(QT_NO_DEBUG)
#  define ENABLE_TMP_LOG
#endif

#if defined(CHECK_MEM_LEAK)
#include <set>
#  define DECL_CLASS_SET(CLASSNAME) \
    static std::set<CLASSNAME *> s_##CLASSNAME##s;

#  define HANDLE_CONSTRUCT(CLASSNAME) \
    do { \
        TM_MARK("create %s: (%p)", #CLASSNAME, this); \
        bool res = s_##CLASSNAME##s.insert(this).second; \
        TM_DBG("%s to insert %s, size now: %d", res? "success": "fail", #CLASSNAME, s_##CLASSNAME##s.size()); \
    } while (0)

#  define HANDLE_DESTRUCT(CLASSNAME) \
    do { \
        TM_MARK("destroy %s: (%p)", #CLASSNAME, this); \
        size_t n = s_##CLASSNAME##s.erase(this); \
        TM_DBG("%d %s erased, size now: %d", n, #CLASSNAME, s_##CLASSNAME##s.size()); \
    } while (0)
#else
#  define DECL_CLASS_SET(CLASSNAME)
#  define HANDLE_CONSTRUCT(CLASSNAME)
#  define HANDLE_DESTRUCT(CLASSNAME)
#endif


#ifdef ENABLE_TMP_LOG
#  ifdef QT_CORE_LIB
#    define TM_DBG \
    (tmp_log().no_space() << "   Loc: [" __FILE__ << "(" << __LINE__ << ")]:").space()
#  else
#    define TM_DBG  (tmp_log().no_space() << "\t").space()
#  endif
#else
#  define TM_DBG  no_tmp_log()
#endif

#ifdef __GNUG__
#  define __FUNC_NAME__   __PRETTY_FUNCTION__
#else
/// \todo
#endif

#define TM_MARK (TM_DBG << __FUNC_NAME__)


#ifdef ENABLE_TMP_LOG

/*!
 * \brief The tmp_log class
 */
class tmp_log
{
public:
    tmp_log() : m_os(new stream()) {}

    ~tmp_log() {
        if (m_os) {
            try {
                std::cerr << m_os->ss.str() << std::endl << std::flush;
            } catch (...) {}
            delete m_os;
            m_os = NULL;
        }
    }

public:
    inline tmp_log &space() {
        m_os->space = true;
        m_os->ss << ' ';
        return *this;
    }

    inline tmp_log &no_space() {
        m_os->space = false;
        return *this;
    }

    inline tmp_log &maybe_space() {
        if (m_os->space) {
            m_os->ss << ' ';
        }
        return *this;
    }

public:
    inline tmp_log &operator()(const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        char buff[256] = { '\0' };
        vsprintf(buff, fmt, ap);
        va_end(ap);
        m_os->ss << buff;
        return maybe_space();
    }

    template<typename T>
    inline tmp_log &operator<<(const T &t) {
        m_os->ss << t;
        return maybe_space();
    }

    inline tmp_log &operator<<(bool t) {
        m_os->ss << (t? "true": "false");
        return maybe_space();
    }

private:
    struct stream {
        stream() : space(true) {}
        std::ostringstream ss;  // text stream
        bool space;
    } *m_os;
};


#else

/*!
 * \brief The no_tmp_log class
 */
class no_tmp_log
{
public:
    inline no_tmp_log &space() { return *this; }
    inline no_tmp_log &no_space() { return *this; }
    inline no_tmp_log &maybe_space() { return *this; }

public:
    inline no_tmp_log &operator ()(const char *, ...) { return *this; }

    template<typename T>
    inline no_tmp_log &operator<<(const T &) { return *this; }
};

#endif // ENABLE_TMP_LOG


#endif // KV_MS_TMP_LOG_H
