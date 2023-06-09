#ifndef UTILS_H
#define UTILS_H

template <typename T>
void enumToQCombobox(QComboBox *comboBox, QMetaEnum &metaEnum)
{
    metaEnum = QMetaEnum::fromType<T>();
    for(int i = 0; i < metaEnum.keyCount(); i++)
    {
        comboBox->addItem(QString(metaEnum.key(i)));
    }
}

template <typename T>
T comboBoxToEnum(QComboBox *comboBox, QMetaEnum &metaEnum)
{
    QString key = comboBox->currentText();
    std::string keyStdString = key.toStdString();
    int value = metaEnum.keyToValue(keyStdString.data());
    return static_cast<T>(value);
}

#endif // UTILS_H
