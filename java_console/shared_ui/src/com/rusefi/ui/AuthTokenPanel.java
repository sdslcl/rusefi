package com.rusefi.ui;

import com.rusefi.auth.AutoTokenUtil;
import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.ui.util.URLLabel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.awt.datatransfer.*;
import java.awt.event.ActionEvent;
import java.io.IOException;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class AuthTokenPanel {
    private static final String TOKEN_WARNING = "Please copy token from your forum profile";
    private static final String AUTH_TOKEN = "auth_token";
    private static final String TOKEN_PROFILE_URL = "https://rusefi.com/forum/ucp.php?i=254";

    private final JPanel content = new JPanel(new BorderLayout());
    private final JTextField authTokenTestField = new JTextField();

    public AuthTokenPanel() {

        JPanel top = new JPanel(new FlowLayout(FlowLayout.LEFT));

        content.setBorder(BorderFactory.createTitledBorder("rusEFI Online Authentication Token"));

        authTokenTestField.setPreferredSize(new Dimension(200, 24));

        String authToken = getAuthToken();
        System.out.println("Got from settings: " + authToken);

        authTokenTestField.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                onTextChange();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                onTextChange();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                onTextChange();
            }
        });

        final Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();

        JButton paste = new JButton("Paste from clipboard");
        paste.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    String data = (String) clipboard.getData(DataFlavor.stringFlavor);
                    if (AutoTokenUtil.isToken(data)) {
                        authTokenTestField.setText(data);
                    }
                } catch (IOException | UnsupportedFlavorException ex) {
                    // ignoring this exception
                }
            }
        });

        clipboard.addFlavorListener(e -> {
            setPasteButtonEnabledBasedOnClipboardContent(clipboard, paste);
        });

        setPasteButtonEnabledBasedOnClipboardContent(clipboard, paste);

        top.add(authTokenTestField);
        top.add(paste);
/*
        JButton save = new JButton("Save");
        save.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                grabText();
            }
        });
        top.add(save);
*/
        content.add(top);
        if (authToken.trim().isEmpty()) {
            authToken = TOKEN_WARNING;
        }
        content.add(new URLLabel("Manage authentication token at your forum profile", TOKEN_PROFILE_URL), BorderLayout.SOUTH);
        authTokenTestField.setText(authToken);
    }

    private void setPasteButtonEnabledBasedOnClipboardContent(Clipboard clipboard, JButton paste) {
        try {
            String data = (String) clipboard.getData(DataFlavor.stringFlavor);
            paste.setEnabled(AutoTokenUtil.isToken(data));
        } catch (IOException | IllegalStateException | UnsupportedFlavorException ex) {
            // ignoring this exception
        }
    }

    private void grabText() {
        setAuthToken(AuthTokenPanel.this.authTokenTestField.getText());
        PersistentConfiguration.getConfig().save();
    }

    private void onTextChange() {
        if (AutoTokenUtil.isToken(authTokenTestField.getText())) {
            grabText();
        }
    }

    public static void setAuthToken(String value) {
        getConfig().getRoot().setProperty(AUTH_TOKEN, value);
    }

    @NotNull
    public static String getAuthToken() {
        return getConfig().getRoot().getProperty(AUTH_TOKEN);
    }

    public JPanel getContent() {
        return content;
    }

    public boolean hasToken() {
        return AutoTokenUtil.isToken(authTokenTestField.getText());
    }

    public String getToken() {
        return authTokenTestField.getText();
    }

    public void showError(JComponent parent) {
        JOptionPane.showMessageDialog(parent, "Does not work without auth token, see below.");
    }
}
