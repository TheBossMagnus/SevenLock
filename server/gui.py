import tkinter as tk
import csv
import os

passwordsFile = "passwords.csv"

def load_passwords():
    if not os.path.exists(passwordsFile):
        open(passwordsFile, 'w').close()
    with open(passwordsFile, 'r', newline='') as f:
        return [row[0] for row in csv.reader(f) if row]

def save_passwords(passwords):
    with open(passwordsFile, 'w', newline='') as f:
        writer = csv.writer(f)
        for p in passwords:
            writer.writerow([p])

def add_password():
    new_pass = entry.get().strip()
    if new_pass:
        pwds = load_passwords()
        pwds.append(new_pass)
        save_passwords(pwds)
        listbox.insert(tk.END, new_pass)
        entry.delete(0, tk.END)

def remove_password():
    sel = listbox.curselection()
    if sel:
        index = sel[0]
        pwds = load_passwords()
        del pwds[index]
        save_passwords(pwds)
        listbox.delete(index)

root = tk.Tk()
root.title("Gestione Password")
entry = tk.Entry(root)
entry.pack()

btn_add = tk.Button(root, text="Aggiungi", command=add_password)
btn_add.pack()

listbox = tk.Listbox(root)
listbox.pack()

btn_remove = tk.Button(root, text="Rimuovi", command=remove_password)
btn_remove.pack()

for p in load_passwords():
    listbox.insert(tk.END, p)

root.mainloop()