//=============================================================================
/// Copyright (c) 2017-2020 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief Implementation for a mini recent trace widget.
//=============================================================================

#include "recent_trace_mini_widget.h"

#include <QVBoxLayout>
#include <QSignalMapper>
#include <QDir>
#include <QEvent>
#include <time.h>

#include "utils/common_definitions.h"

RecentTraceMiniWidget::RecentTraceMiniWidget(QWidget* parent)
    : QWidget(parent)
{
    path_signal_mapper_ = new QSignalMapper(this);
    path_button_        = new ScaledPushButton(this);

    path_button_->setCursor(Qt::PointingHandCursor);
    path_button_->setStyleSheet(kLinkButtonStylesheet);

    // Additional settings for the path button
    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    path_button_->setSizePolicy(policy);

    // Create the layout
    widget_layout_ = new QVBoxLayout(this);
    widget_layout_->setContentsMargins(0, 0, 0, 0);
    widget_layout_->setSpacing(0);
}

RecentTraceMiniWidget::~RecentTraceMiniWidget()
{
    delete path_signal_mapper_;
    delete path_button_;
}

void RecentTraceMiniWidget::changeEvent(QEvent* event)
{
    if (event != nullptr)
    {
        if (event->type() == QEvent::FontChange)
        {
            path_button_->setFont(this->font());
        }
    }

    // Call the base class implementation to handle all default functionality.
    QWidget::changeEvent(event);
}

void RecentTraceMiniWidget::SetFile(const RecentFileData& file)
{
    widget_layout_->addWidget(path_button_);

    // Get file parameters. Convert file from its native separators
    QString path     = QDir::fromNativeSeparators(file.path);
    QString fileName = path.mid(path.lastIndexOf("/") + 1, path.length());

    // Set label text as just the filename and tooltip as full path
    path_button_->setText(fileName);
    path_button_->setToolTip(path);

    // Connect path label clicked() signal to this widgets clicked(QString) signal
    path_signal_mapper_->setMapping(path_button_, path);
    connect(path_button_, SIGNAL(clicked()), path_signal_mapper_, SLOT(map()));
    connect(path_signal_mapper_, SIGNAL(mapped(QString)), this, SIGNAL(clicked(QString)));
}

QString RecentTraceMiniWidget::GetPath() const
{
    // The tooltip contains the full path string.
    return path_button_->toolTip();
}

QSize RecentTraceMiniWidget::sizeHint() const
{
    return path_button_->sizeHint();
}
