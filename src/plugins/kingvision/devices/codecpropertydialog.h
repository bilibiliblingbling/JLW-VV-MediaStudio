#ifndef CODEC_PROPERTY_DIALOG_H
#define CODEC_PROPERTY_DIALOG_H

#include <QDialog>

namespace KingVision {
namespace Internal {

class Codec;

class CodecPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    CodecPropertyDialog(const Codec &codec, QWidget *parent = 0);
};

}
}


#endif // CODEC_PROPERTY_DIALOG_H

